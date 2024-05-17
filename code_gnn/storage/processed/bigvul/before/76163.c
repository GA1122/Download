init_data(const char *conf_file, vector_t * (*init_keywords) (void))
{
	 
	keywords = vector_alloc();

	(*init_keywords) ();

	 
	set_std_definitions();

#if DUMP_KEYWORDS
	 
	dump_keywords(keywords, 0, NULL);
#endif

	 
	current_keywords = keywords;

	current_file_name = NULL;
	current_file_line_no = 0;

	 
	block_depth = 0;
	kw_level = 0;

	register_null_strvec_handler(null_strvec);
	read_conf_file(conf_file);
	unregister_null_strvec_handler();

	 
	if (block_depth > 0)
		report_config_error(CONFIG_MISSING_EOB, "There are %d missing '%s's or extra '%s's", block_depth, EOB, BOB);

	 
	current_file_line_no = 0;

	 
	endpwent();

	free_keywords(keywords);
	free_parser_data();
#ifdef _WITH_VRRP_
	clear_rt_names();
#endif
	notify_resource_release();
}
