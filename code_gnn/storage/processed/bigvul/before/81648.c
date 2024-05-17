dir_scan_callback(struct de *de, void *data)
{
	struct dir_scan_data *dsd = (struct dir_scan_data *)data;

	if ((dsd->entries == NULL) || (dsd->num_entries >= dsd->arr_size)) {
		dsd->arr_size *= 2;
		dsd->entries =
		    (struct de *)realloc2(dsd->entries,
		                          dsd->arr_size * sizeof(dsd->entries[0]));
	}
	if (dsd->entries == NULL) {
		 
		dsd->num_entries = 0;
	} else {
		dsd->entries[dsd->num_entries].file_name = mg_strdup(de->file_name);
		dsd->entries[dsd->num_entries].file = de->file;
		dsd->entries[dsd->num_entries].conn = de->conn;
		dsd->num_entries++;
	}

	return 0;
}