static int str_hash(void *key) {
 return hashmapHash(key, strlen(key));
}
