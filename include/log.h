#ifndef __LOG_H__
#define __LOG_H__

#define LOG_LEVEL_ERROR   0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_DEBUG   2
#define LOG_LEVEL_INFO    3

#ifdef CONFIG_DEBUG_INFO
extern void print_log(int level, const char *fmt, ...);
#define log(level, fmt, ...)  print_log(level, fmt "\r\n", ##__VA_ARGS__)
#else
#define log(level, fmt, ...) do { } while (0)
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL -1
#endif

#if (LOG_LEVEL >= LOG_LEVEL_ERROR)
#define log_e(fmt, ...)  log(LOG_LEVEL_ERROR, "[E]: " fmt, ##__VA_ARGS__)
#else
#define log_e(fmt, ...) do { } while (0)
#endif
#if (LOG_LEVEL >= LOG_LEVEL_WARNING)
#define log_w(fmt, ...)  log(LOG_LEVEL_WARNING, "[W]: " fmt, ##__VA_ARGS__)
#else
#define log_w(fmt, ...) do { } while (0)
#endif
#if (LOG_LEVEL >= LOG_LEVEL_DEBUG)
#define log_d(fmt, ...)  log(LOG_LEVEL_DEBUG, "[D]: " fmt, ##__VA_ARGS__)
#else
#define log_d(fmt, ...) do { } while (0)
#endif
#if (LOG_LEVEL >= LOG_LEVEL_INFO)
#define log_i(fmt, ...)  log(LOG_LEVEL_INFO, "[I]: " fmt, ##__VA_ARGS__)
#else
#define log_i(fmt, ...) do { } while (0)
#endif

#endif /* __LOG_H__ */