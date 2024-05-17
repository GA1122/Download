config_t *config_new_empty(void) {
 config_t *config = osi_calloc(sizeof(config_t));
 if (!config) {
    LOG_ERROR("%s unable to allocate memory for config_t.", __func__);
 goto error;
 }

  config->sections = list_new(section_free);
 if (!config->sections) {
    LOG_ERROR("%s unable to allocate list for sections.", __func__);
 goto error;
 }

 return config;

error:;
  config_free(config);
 return NULL;
}
