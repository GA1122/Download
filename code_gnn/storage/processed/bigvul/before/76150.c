check_include(char *buf)
{
	vector_t *strvec;
	bool ret = false;
	FILE *prev_stream;
	const char *prev_file_name;
	size_t prev_file_line_no;

	 
	if (!strstr(buf, "include"))
		return false;

	strvec = alloc_strvec(buf);

	if (!strvec)
		return false;

	if(!strcmp("include", vector_slot(strvec, 0)) && vector_size(strvec) == 2) {
		prev_stream = current_stream;
		prev_file_name = current_file_name;
		prev_file_line_no = current_file_line_no;

		read_conf_file(vector_slot(strvec, 1));

		current_stream = prev_stream;
		current_file_name = prev_file_name;
		current_file_line_no = prev_file_line_no;

		ret = true;
	}

	free_strvec(strvec);
	return ret;
}