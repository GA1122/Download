static int get_sub_api(char* driver, int api){
	int i;
	const char sep_str[2] = {LIST_SEPARATOR, 0};
	char *tok, *tmp_str;
	size_t len = safe_strlen(driver);

	if (len == 0) return SUB_API_NOTSET;
	tmp_str = (char*) calloc(len+1, 1);
	if (tmp_str == NULL) return SUB_API_NOTSET;
	memcpy(tmp_str, driver, len+1);
	tok = strtok(tmp_str, sep_str);
	while (tok != NULL) {
		for (i=0; i<usb_api_backend[api].nb_driver_names; i++) {
			if (safe_stricmp(tok, usb_api_backend[api].driver_name_list[i]) == 0) {
				free(tmp_str);
				return i;
			}
		}
		tok = strtok(NULL, sep_str);
	}
	free (tmp_str);
	return SUB_API_NOTSET;
}
