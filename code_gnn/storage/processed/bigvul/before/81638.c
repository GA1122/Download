compare_dir_entries(const void *p1, const void *p2)
{
	if (p1 && p2) {
		const struct de *a = (const struct de *)p1, *b = (const struct de *)p2;
		const char *query_string = a->conn->request_info.query_string;
		int cmp_result = 0;

		if (query_string == NULL) {
			query_string = "na";
		}

		if (a->file.is_directory && !b->file.is_directory) {
			return -1;  
		} else if (!a->file.is_directory && b->file.is_directory) {
			return 1;  
		} else if (*query_string == 'n') {
			cmp_result = strcmp(a->file_name, b->file_name);
		} else if (*query_string == 's') {
			cmp_result = (a->file.size == b->file.size)
			                 ? 0
			                 : ((a->file.size > b->file.size) ? 1 : -1);
		} else if (*query_string == 'd') {
			cmp_result =
			    (a->file.last_modified == b->file.last_modified)
			        ? 0
			        : ((a->file.last_modified > b->file.last_modified) ? 1
			                                                           : -1);
		}

		return (query_string[1] == 'd') ? -cmp_result : cmp_result;
	}
	return 0;
}