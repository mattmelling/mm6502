#include <stdio.h>

#define LOG_FATAL    (1)
#define LOG_ERR      (2)
#define LOG_WARN     (3)
#define LOG_INFO     (4)
#define LOG_DEBUG    (5)
#define LOG_TRACE    (6)

#define LOG(level, type, ...) do {                       \
                            if (level <= debug_level) { \
                                printf("%s:%s:%d:", type, __FILE__, __LINE__); \
                                printf(__VA_ARGS__); \
                                printf("\n"); \
                            } \
                        } while (0)
#define debug_level LOG_DEBUG

#define trace(...) LOG(LOG_TRACE, "trace",__VA_ARGS__)
#define debug(...) LOG(LOG_DEBUG, "debug",__VA_ARGS__)
#define info(...) LOG(LOG_INFO, "info",__VA_ARGS__)
#define warn(...) LOG(LOG_WARN, "warn",__VA_ARGS__)
