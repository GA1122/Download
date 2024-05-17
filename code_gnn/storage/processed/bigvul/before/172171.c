static section_t *section_find(const config_t *config, const char *section) {
 for (const list_node_t *node = list_begin(config->sections); node != list_end(config->sections); node = list_next(node)) {
 section_t *sec = list_node(node);
 if (!strcmp(sec->name, section))
 return sec;
 }

 return NULL;
}
