static void btif_config_remove_unpaired(config_t *conf) {
  assert(conf != NULL);

 const config_section_node_t *snode = config_section_begin(conf);
 while (snode != config_section_end(conf)) {
 const char *section = config_section_name(snode);
 if (string_is_bdaddr(section)) {
 if (!config_has_key(conf, section, "LinkKey") &&
 !config_has_key(conf, section, "LE_KEY_PENC") &&
 !config_has_key(conf, section, "LE_KEY_PID") &&
 !config_has_key(conf, section, "LE_KEY_PCSRK") &&
 !config_has_key(conf, section, "LE_KEY_LENC") &&
 !config_has_key(conf, section, "LE_KEY_LCSRK")) {
        snode = config_section_next(snode);
        config_remove_section(conf, section);
 continue;
 }
 }

     snode = config_section_next(snode);
   }
 }
