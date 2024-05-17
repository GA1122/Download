static uintmax_t do_change_note_fanout(
		struct tree_entry *orig_root, struct tree_entry *root,
		char *hex_sha1, unsigned int hex_sha1_len,
		char *fullpath, unsigned int fullpath_len,
		unsigned char fanout)
{
	struct tree_content *t = root->tree;
	struct tree_entry *e, leaf;
	unsigned int i, tmp_hex_sha1_len, tmp_fullpath_len;
	uintmax_t num_notes = 0;
	unsigned char sha1[20];
	char realpath[60];

	for (i = 0; t && i < t->entry_count; i++) {
		e = t->entries[i];
		tmp_hex_sha1_len = hex_sha1_len + e->name->str_len;
		tmp_fullpath_len = fullpath_len;

		 
		if (!e->versions[1].mode ||
		    tmp_hex_sha1_len > 40 ||
		    e->name->str_len % 2)
			continue;

		 
		memcpy(hex_sha1 + hex_sha1_len, e->name->str_dat,
		       e->name->str_len);
		if (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		memcpy(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		if (tmp_hex_sha1_len == 40 && !get_sha1_hex(hex_sha1, sha1)) {
			 
			if (fanout == 0xff) {
				 
				num_notes++;
				continue;
			}
			construct_path_with_fanout(hex_sha1, fanout, realpath);
			if (!strcmp(fullpath, realpath)) {
				 
				num_notes++;
				continue;
			}

			 
			if (!tree_content_remove(orig_root, fullpath, &leaf, 0))
				die("Failed to remove path %s", fullpath);
			tree_content_set(orig_root, realpath,
				leaf.versions[1].sha1,
				leaf.versions[1].mode,
				leaf.tree);
		} else if (S_ISDIR(e->versions[1].mode)) {
			 
			if (!e->tree)
				load_tree(e);
			num_notes += do_change_note_fanout(orig_root, e,
				hex_sha1, tmp_hex_sha1_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		 
		t = root->tree;
	}
	return num_notes;
}
