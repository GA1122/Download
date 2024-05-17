config_t *config_new(const char *filename) {
  assert(filename != NULL);

 config_t *config = config_new_empty();
 if (!config)
 return NULL;

 FILE *fp = fopen(filename, "rt");
 if (!fp) {
    LOG_ERROR("%s unable to open file '%s': %s", __func__, filename, strerror(errno));
    config_free(config);
 return NULL;
 }
  config_parse(fp, config);
  fclose(fp);
 return config;
}
