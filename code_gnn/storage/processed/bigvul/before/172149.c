void config_free(config_t *config) {
 if (!config)
 return;

  list_free(config->sections);
  osi_free(config);
}
