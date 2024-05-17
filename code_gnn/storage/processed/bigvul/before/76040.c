vrrp_tfile_init_handler(vector_t *strvec)
{
	unsigned i;
	char *word;
	vrrp_tracked_file_t *tfile = LIST_TAIL_DATA(vrrp_data->vrrp_track_files);
	int value;

	track_file_init = TRACK_FILE_CREATE;
	track_file_init_value = 0;

	for (i = 1; i < vector_size(strvec); i++) {
		word = strvec_slot(strvec, i);
		word += strspn(word, WHITE_SPACE);
		if (isdigit(word[0]) || word[0] == '-') {
			if (!read_int_strvec(strvec, i, &value, INT_MIN, INT_MAX, false)) {
				 
				report_config_error(CONFIG_GENERAL_ERROR, "Track file %s init value %s is invalid", tfile->fname, word);
				value = 0;
			}
			else if (value < -254 || value > 254)
				report_config_error(CONFIG_GENERAL_ERROR, "Track file %s init value %d is outside sensible range [%d, %d]", tfile->fname, value, -254, 254);
			track_file_init_value = value;
		}
		else if (!strcmp(word, "overwrite"))
			track_file_init = TRACK_FILE_INIT;
		else
			report_config_error(CONFIG_GENERAL_ERROR, "Unknown track file init option %s", word);
	}
}