static inline char *_ascii_get_suffix_buf(conn *c, int i) {
    char *suffix;
     
    if (i >= c->suffixsize) {
    char **new_suffix_list = realloc(c->suffixlist,
                           sizeof(char *) * c->suffixsize * 2);
    if (new_suffix_list) {
        c->suffixsize *= 2;
        c->suffixlist  = new_suffix_list;
    } else {
        STATS_LOCK();
        stats.malloc_fails++;
        STATS_UNLOCK();
        return NULL;
    }
    }

    suffix = do_cache_alloc(c->thread->suffix_cache);
    if (suffix == NULL) {
      STATS_LOCK();
      stats.malloc_fails++;
      STATS_UNLOCK();
      out_of_memory(c, "SERVER_ERROR out of memory making CAS suffix");
      return NULL;
    }
    *(c->suffixlist + i) = suffix;
    return suffix;
}