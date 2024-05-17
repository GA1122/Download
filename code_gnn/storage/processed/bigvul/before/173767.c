static bool remove_str_to_int(void *key, void *value, void *context) {
 Hashmap* map = context;
    hashmapRemove(map, key);
    free(key);
 return true;
}
