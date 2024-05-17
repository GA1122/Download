slurp_central_directory(struct archive_read *a, struct zip *zip)
{
	ssize_t i;
	unsigned found;
	int64_t correction;
	ssize_t bytes_avail;
	const char *p;

	 
	if (__archive_read_seek(a, zip->central_directory_offset, SEEK_SET) < 0)
		return ARCHIVE_FATAL;

	found = 0;
	while (!found) {
		if ((p = __archive_read_ahead(a, 20, &bytes_avail)) == NULL)
			return ARCHIVE_FATAL;
		for (found = 0, i = 0; !found && i < bytes_avail - 4;) {
			switch (p[i + 3]) {
			case 'P': i += 3; break;
			case 'K': i += 2; break;
			case 001: i += 1; break;
			case 002:
				if (memcmp(p + i, "PK\001\002", 4) == 0) {
					p += i;
					found = 1;
				} else
					i += 4;
				break;
			case 005: i += 1; break;
			case 006:
				if (memcmp(p + i, "PK\005\006", 4) == 0) {
					p += i;
					found = 1;
				} else if (memcmp(p + i, "PK\006\006", 4) == 0) {
					p += i;
					found = 1;
				} else
					i += 1;
				break;
			default: i += 4; break;
			}
		}
		__archive_read_consume(a, i);
	}
	correction = archive_filter_bytes(&a->archive, 0)
			- zip->central_directory_offset;

	__archive_rb_tree_init(&zip->tree, &rb_ops);
	__archive_rb_tree_init(&zip->tree_rsrc, &rb_rsrc_ops);

	zip->central_directory_entries_total = 0;
	while (1) {
		struct zip_entry *zip_entry;
		size_t filename_length, extra_length, comment_length;
		uint32_t external_attributes;
		const char *name, *r;

		if ((p = __archive_read_ahead(a, 4, NULL)) == NULL)
			return ARCHIVE_FATAL;
		if (memcmp(p, "PK\006\006", 4) == 0
		    || memcmp(p, "PK\005\006", 4) == 0) {
			break;
		} else if (memcmp(p, "PK\001\002", 4) != 0) {
			archive_set_error(&a->archive,
			    -1, "Invalid central directory signature");
			return ARCHIVE_FATAL;
		}
		if ((p = __archive_read_ahead(a, 46, NULL)) == NULL)
			return ARCHIVE_FATAL;

		zip_entry = calloc(1, sizeof(struct zip_entry));
		zip_entry->next = zip->zip_entries;
		zip_entry->flags |= LA_FROM_CENTRAL_DIRECTORY;
		zip->zip_entries = zip_entry;
		zip->central_directory_entries_total++;

		 
		zip_entry->system = p[5];
		 
		zip_entry->zip_flags = archive_le16dec(p + 8);
		if (zip_entry->zip_flags
		      & (ZIP_ENCRYPTED | ZIP_STRONG_ENCRYPTED)){
			zip->has_encrypted_entries = 1;
		}
		zip_entry->compression = (char)archive_le16dec(p + 10);
		zip_entry->mtime = zip_time(p + 12);
		zip_entry->crc32 = archive_le32dec(p + 16);
		if (zip_entry->zip_flags & ZIP_LENGTH_AT_END)
			zip_entry->decdat = p[13];
		else
			zip_entry->decdat = p[19];
		zip_entry->compressed_size = archive_le32dec(p + 20);
		zip_entry->uncompressed_size = archive_le32dec(p + 24);
		filename_length = archive_le16dec(p + 28);
		extra_length = archive_le16dec(p + 30);
		comment_length = archive_le16dec(p + 32);
		   
		   
		external_attributes = archive_le32dec(p + 38);
		zip_entry->local_header_offset =
		    archive_le32dec(p + 42) + correction;

		 
		if (zip_entry->system == 3) {
			zip_entry->mode = external_attributes >> 16;
		} else if (zip_entry->system == 0) {
			if (0x10 == (external_attributes & 0x10)) {
				zip_entry->mode = AE_IFDIR | 0775;
			} else {
				zip_entry->mode = AE_IFREG | 0664;
			}
			if (0x01 == (external_attributes & 0x01)) {
				zip_entry->mode &= 0555;
			}
		} else {
			zip_entry->mode = 0;
		}

		 
		__archive_read_consume(a, 46);
		p = __archive_read_ahead(a, filename_length + extra_length,
			NULL);
		if (p == NULL) {
			archive_set_error(&a->archive,
			    ARCHIVE_ERRNO_FILE_FORMAT,
			    "Truncated ZIP file header");
			return ARCHIVE_FATAL;
		}
		process_extra(p + filename_length, extra_length, zip_entry);

		 
		if (!zip->process_mac_extensions) {
			 
			__archive_rb_tree_insert_node(&zip->tree,
			    &zip_entry->node);
		} else {
			name = p;
			r = rsrc_basename(name, filename_length);
			if (filename_length >= 9 &&
			    strncmp("__MACOSX/", name, 9) == 0) {
				 
				if (name[filename_length-1] != '/' &&
				    (r - name < 3 || r[0] != '.' || r[1] != '_')) {
					__archive_rb_tree_insert_node(
					    &zip->tree, &zip_entry->node);
					 
					expose_parent_dirs(zip, name,
					    filename_length);
				} else {
					 
					archive_strncpy(&(zip_entry->rsrcname),
					     name, filename_length);
					__archive_rb_tree_insert_node(
					    &zip->tree_rsrc, &zip_entry->node);
				}
			} else {
				 
				archive_strcpy(&(zip_entry->rsrcname),
				    "__MACOSX/");
				archive_strncat(&(zip_entry->rsrcname),
				    name, r - name);
				archive_strcat(&(zip_entry->rsrcname), "._");
				archive_strncat(&(zip_entry->rsrcname),
				    name + (r - name),
				    filename_length - (r - name));
				 
				__archive_rb_tree_insert_node(&zip->tree,
				    &zip_entry->node);
			}
		}

		 
		__archive_read_consume(a,
		    filename_length + extra_length + comment_length);
	}

	return ARCHIVE_OK;
}