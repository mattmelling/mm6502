#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <pty.h>

#include "acia.h"
#include "log.h"

uint8_t mm6502_acia_read_callback(fake6502_context *context, uint16_t address, void *data)
{
  mm6551_acia *acia = (mm6551_acia *)data;
  uint16_t reg = address - acia->address;
  switch(reg) {
  case ACIA_REG_BASE:
    acia->status ^= ACIA_STATUS_RXF;
    return acia->rx;
  case ACIA_REG_STATUS:
    return acia->status;
  case ACIA_REG_COMMAND:
    return acia->command;
  case ACIA_REG_CONTROL:
    return acia->control;
  }
  return 0;
}

void mm6502_acia_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data)
{
  mm6551_acia *acia = (mm6551_acia *)data;
  uint16_t reg = address - acia->address;
  switch(reg) {
  case ACIA_REG_BASE:
    acia->tx = value;
    acia->status ^= ACIA_STATUS_TXE;
    return;
  case ACIA_REG_COMMAND:
    acia->command = value;
    return;
  case ACIA_REG_CONTROL:
    acia->control = value;
    return;
  default:
    return;
  }
}

void mm6551_init(mm6551_acia *acia)
{

  // create pty
  int amaster, aslave;
  char *name = malloc(sizeof(char) * 256);
  openpty(&amaster, &aslave, name, NULL, NULL);

  // set non blocking
  int flags = fcntl(amaster, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(amaster, F_SETFL, flags);

  // set up polling for step callback
  acia->fd.fd = amaster;
  acia->fd.events = POLLOUT | POLLIN;

  info("acia at %x on %s", acia->address, name);
}

void mm6551_step_callback(fake6502_context *context, void *data)
{
  mm6551_acia *acia = (mm6551_acia *)data;
  char buf;
  ssize_t n;

  // todo recover from terminal disconnect

  if(poll(&acia->fd, 1, 100) > 0) {

    // can we read?
    if(acia->fd.revents & POLLIN) {
      if(read(acia->fd.fd, &buf, 1) > 0) {
        // have we overrun?
        if(acia->status & ACIA_STATUS_RXF) {
          acia->status |= ACIA_STATUS_OVR;
        }

        acia->rx = buf;
        acia->status |= ACIA_STATUS_RXF;

        // interrupt?
        if(acia->command & ~ACIA_COMMAND_IRD) {
          acia->status |= ACIA_STATUS_IRQ;
          fake6502_irq(context);
        }
      }
    }

    // can we write?
    if(acia->fd.revents & POLLOUT) {

      if((acia->status & ACIA_STATUS_TXE) == 0) {
        // only if the tx buffer is full though
        n = write(acia->fd.fd, &acia->tx, 1);

        acia->status |= ACIA_STATUS_TXE;

        if(acia->command & ~ACIA_COMMAND_TICH & ACIA_COMMAND_TICL) {
          acia->status |= ACIA_STATUS_IRQ;
          fake6502_irq(context);
        }
      }
    }
  }
}
