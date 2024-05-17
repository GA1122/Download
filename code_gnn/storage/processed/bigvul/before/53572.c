read_stream(struct archive_read *a, const void **buff, size_t size,
    size_t minimum)
{
	struct _7zip *zip = (struct _7zip *)a->format->data;
	uint64_t skip_bytes = 0;
	ssize_t r;

	if (zip->uncompressed_buffer_bytes_remaining == 0) {
		if (zip->pack_stream_inbytes_remaining > 0) {
			r = extract_pack_stream(a, 0);
			if (r < 0)
				return (r);
			return (get_uncompressed_data(a, buff, size, minimum));
		} else if (zip->folder_outbytes_remaining > 0) {
			 
			r = extract_pack_stream(a, 0);
			if (r < 0)
				return (r);
			return (get_uncompressed_data(a, buff, size, minimum));
		}
	} else
		return (get_uncompressed_data(a, buff, size, minimum));

	 
	if (zip->pack_stream_remaining == 0) {
		if (zip->header_is_being_read) {
			 
			archive_set_error(&(a->archive),
			    ARCHIVE_ERRNO_MISC, "Malformed 7-Zip archive");
			return (ARCHIVE_FATAL);
		}

		 
		if (zip->folder_index == 0 &&
		    (zip->si.ci.folders[zip->entry->folderIndex].skipped_bytes
		     || zip->folder_index != zip->entry->folderIndex)) {
			zip->folder_index = zip->entry->folderIndex;
			skip_bytes =
			    zip->si.ci.folders[zip->folder_index].skipped_bytes;
		}

		if (zip->folder_index >= zip->si.ci.numFolders) {
			 
			*buff = NULL;
			return (0);
		}
		r = setup_decode_folder(a,
			&(zip->si.ci.folders[zip->folder_index]), 0);
		if (r != ARCHIVE_OK)
			return (ARCHIVE_FATAL);

		zip->folder_index++;
	}

	 
	r = seek_pack(a);
	if (r < 0)
		return (r);

	 
	r = extract_pack_stream(a, 0);
	if (r < 0)
		return (r);

	 
	while (skip_bytes) {
		ssize_t skipped;

		if (zip->uncompressed_buffer_bytes_remaining == 0) {
			if (zip->pack_stream_inbytes_remaining > 0) {
				r = extract_pack_stream(a, 0);
				if (r < 0)
					return (r);
			} else if (zip->folder_outbytes_remaining > 0) {
				 
				r = extract_pack_stream(a, 0);
				if (r < 0)
					return (r);
			} else {
				archive_set_error(&a->archive,
				    ARCHIVE_ERRNO_FILE_FORMAT,
				    "Truncated 7-Zip file body");
				return (ARCHIVE_FATAL);
			}
		}
		skipped = get_uncompressed_data(
			a, buff, (size_t)skip_bytes, 0);
		if (skipped < 0)
			return (skipped);
		skip_bytes -= skipped;
		if (zip->pack_stream_bytes_unconsumed)
			read_consume(a);
	}

	return (get_uncompressed_data(a, buff, size, minimum));
}
