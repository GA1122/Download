expose_parent_dirs(struct zip *zip, const char *name, size_t name_length)
{
	struct archive_string str;
	struct zip_entry *dir;
	char *s;

	archive_string_init(&str);
	archive_strncpy(&str, name, name_length);
	for (;;) {
		s = strrchr(str.s, '/');
		if (s == NULL)
			break;
		*s = '\0';
		 
		dir = (struct zip_entry *)
		    __archive_rb_tree_find_node(&zip->tree_rsrc, str.s);
		if (dir == NULL)
			break;
		__archive_rb_tree_remove_node(&zip->tree_rsrc, &dir->node);
		archive_string_free(&dir->rsrcname);
		__archive_rb_tree_insert_node(&zip->tree, &dir->node);
	}
	archive_string_free(&str);
}
