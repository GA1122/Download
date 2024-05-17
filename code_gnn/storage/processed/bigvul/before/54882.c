static unsigned long write_no_reuse_object(struct sha1file *f, struct object_entry *entry,
					   unsigned long limit, int usable_delta)
{
	unsigned long size, datalen;
	unsigned char header[10], dheader[10];
	unsigned hdrlen;
	enum object_type type;
	void *buf;
	struct git_istream *st = NULL;

	if (!usable_delta) {
		if (entry->type == OBJ_BLOB &&
		    entry->size > big_file_threshold &&
		    (st = open_istream(entry->idx.sha1, &type, &size, NULL)) != NULL)
			buf = NULL;
		else {
			buf = read_sha1_file(entry->idx.sha1, &type, &size);
			if (!buf)
				die(_("unable to read %s"), sha1_to_hex(entry->idx.sha1));
		}
		 
		free(entry->delta_data);
		entry->delta_data = NULL;
		entry->z_delta_size = 0;
	} else if (entry->delta_data) {
		size = entry->delta_size;
		buf = entry->delta_data;
		entry->delta_data = NULL;
		type = (allow_ofs_delta && entry->delta->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} else {
		buf = get_delta(entry);
		size = entry->delta_size;
		type = (allow_ofs_delta && entry->delta->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	if (st)	 
		datalen = size;
	else if (entry->z_delta_size)
		datalen = entry->z_delta_size;
	else
		datalen = do_compress(&buf, size);

	 
	hdrlen = encode_in_pack_object_header(type, size, header);

	if (type == OBJ_OFS_DELTA) {
		 
		off_t ofs = entry->idx.offset - entry->delta->idx.offset;
		unsigned pos = sizeof(dheader) - 1;
		dheader[pos] = ofs & 127;
		while (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		if (limit && hdrlen + sizeof(dheader) - pos + datalen + 20 >= limit) {
			if (st)
				close_istream(st);
			free(buf);
			return 0;
		}
		sha1write(f, header, hdrlen);
		sha1write(f, dheader + pos, sizeof(dheader) - pos);
		hdrlen += sizeof(dheader) - pos;
	} else if (type == OBJ_REF_DELTA) {
		 
		if (limit && hdrlen + 20 + datalen + 20 >= limit) {
			if (st)
				close_istream(st);
			free(buf);
			return 0;
		}
		sha1write(f, header, hdrlen);
		sha1write(f, entry->delta->idx.sha1, 20);
		hdrlen += 20;
	} else {
		if (limit && hdrlen + datalen + 20 >= limit) {
			if (st)
				close_istream(st);
			free(buf);
			return 0;
		}
		sha1write(f, header, hdrlen);
	}
	if (st) {
		datalen = write_large_blob_data(st, f, entry->idx.sha1);
		close_istream(st);
	} else {
		sha1write(f, buf, datalen);
		free(buf);
	}

	return hdrlen + datalen;
}
