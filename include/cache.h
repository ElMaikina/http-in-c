#ifndef CACHE_H
#define CACHE_H

#define CACHE_HOST "127.0.0.1"
#define CACHE_PORT 6379

char *SearchInCache(const char *key);
void StoreInCache(const char *key, const char *value);

#endif
