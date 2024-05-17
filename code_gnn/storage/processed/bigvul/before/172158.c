static void config_parse(FILE *fp, config_t *config) {
  assert(fp != NULL);
  assert(config != NULL);

 int line_num = 0;
 char line[1024];
 char section[1024];
  strcpy(section, CONFIG_DEFAULT_SECTION);

 while (fgets(line, sizeof(line), fp)) {
 char *line_ptr = trim(line);
 ++line_num;

 if (*line_ptr == '\0' || *line_ptr == '#')
 continue;

 if (*line_ptr == '[') {
 size_t len = strlen(line_ptr);
 if (line_ptr[len - 1] != ']') {
        LOG_DEBUG("%s unterminated section name on line %d.", __func__, line_num);
 continue;
 }
      strncpy(section, line_ptr + 1, len - 2);
      section[len - 2] = '\0';
 } else {
 char *split = strchr(line_ptr, '=');
 if (!split) {
        LOG_DEBUG("%s no key/value separator found on line %d.", __func__, line_num);
 continue;
 }

 *split = '\0';
      config_set_string(config, section, trim(line_ptr), trim(split + 1));
 }
 }
}
