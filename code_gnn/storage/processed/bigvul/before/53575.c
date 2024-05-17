setup_decode_folder(struct archive_read *a, struct _7z_folder *folder,
    int header)
{
	struct _7zip *zip = (struct _7zip *)a->format->data;
	const struct _7z_coder *coder1, *coder2;
	const char *cname = (header)?"archive header":"file content";
	unsigned i;
	int r, found_bcj2 = 0;

	 
	for (i = 0; i < 3; i++) {
		if (zip->sub_stream_buff[i] != NULL)
			free(zip->sub_stream_buff[i]);
		zip->sub_stream_buff[i] = NULL;
	}

	 
	zip->pack_stream_remaining = (unsigned)folder->numPackedStreams;
	zip->pack_stream_index = (unsigned)folder->packIndex;
	zip->folder_outbytes_remaining = folder_uncompressed_size(folder);
	zip->uncompressed_buffer_bytes_remaining = 0;

	 
	for (i = 0; i < folder->numCoders; i++) {
		switch(folder->coders[i].codec) {
			case _7Z_CRYPTO_MAIN_ZIP:
			case _7Z_CRYPTO_RAR_29:
			case _7Z_CRYPTO_AES_256_SHA_256: {
				 
				zip->has_encrypted_entries = 1;
				if (a->entry) {
					archive_entry_set_is_data_encrypted(a->entry, 1);
					archive_entry_set_is_metadata_encrypted(a->entry, 1);
				}
				archive_set_error(&(a->archive),
					ARCHIVE_ERRNO_MISC,
					"The %s is encrypted, "
					"but currently not supported", cname);
				return (ARCHIVE_FATAL);
			}
			case _7Z_X86_BCJ2: {
				found_bcj2++;
				break;
			}
		}
	}
	 
	if (zip->has_encrypted_entries == ARCHIVE_READ_FORMAT_ENCRYPTION_DONT_KNOW) {
		zip->has_encrypted_entries = 0;
	}

	if ((folder->numCoders > 2 && !found_bcj2) || found_bcj2 > 1) {
		archive_set_error(&(a->archive),
		    ARCHIVE_ERRNO_MISC,
		    "The %s is encoded with many filters, "
		    "but currently not supported", cname);
		return (ARCHIVE_FATAL);
	}
	coder1 = &(folder->coders[0]);
	if (folder->numCoders == 2)
		coder2 = &(folder->coders[1]);
	else
		coder2 = NULL;

	if (found_bcj2) {
		 
		const struct _7z_coder *fc = folder->coders;
		static const struct _7z_coder coder_copy = {0, 1, 1, 0, NULL};
		const struct _7z_coder *scoder[3] =
			{&coder_copy, &coder_copy, &coder_copy};
		const void *buff;
		ssize_t bytes;
		unsigned char *b[3] = {NULL, NULL, NULL};
		uint64_t sunpack[3] ={-1, -1, -1};
		size_t s[3] = {0, 0, 0};
		int idx[3] = {0, 1, 2};

		if (folder->numCoders == 4 && fc[3].codec == _7Z_X86_BCJ2 &&
		    folder->numInStreams == 7 && folder->numOutStreams == 4 &&
		    zip->pack_stream_remaining == 4) {
			 
			if (folder->bindPairs[0].inIndex == 5) {
				 
				idx[0] = 1; idx[1] = 2; idx[2] = 0;
				scoder[1] = &(fc[1]);
				scoder[2] = &(fc[0]);
				sunpack[1] = folder->unPackSize[1];
				sunpack[2] = folder->unPackSize[0];
				coder1 = &(fc[2]);
			} else {
				 
				if (fc[0].codec == _7Z_COPY &&
				    fc[1].codec == _7Z_COPY)
					coder1 = &(folder->coders[2]);
				else if (fc[0].codec == _7Z_COPY &&
				    fc[2].codec == _7Z_COPY)
					coder1 = &(folder->coders[1]);
				else if (fc[1].codec == _7Z_COPY &&
				    fc[2].codec == _7Z_COPY)
					coder1 = &(folder->coders[0]);
				else {
					archive_set_error(&(a->archive),
					    ARCHIVE_ERRNO_MISC,
					    "Unsupported form of "
					    "BCJ2 streams");
					return (ARCHIVE_FATAL);
				}
			}
			coder2 = &(fc[3]);
			zip->main_stream_bytes_remaining =
				(size_t)folder->unPackSize[2];
		} else if (coder2 != NULL && coder2->codec == _7Z_X86_BCJ2 &&
		    zip->pack_stream_remaining == 4 &&
		    folder->numInStreams == 5 && folder->numOutStreams == 2) {
			 
			zip->main_stream_bytes_remaining =
				(size_t)folder->unPackSize[0];
		} else {
			 
			archive_set_error(&(a->archive),
			    ARCHIVE_ERRNO_MISC,
			    "Unsupported form of BCJ2 streams");
			return (ARCHIVE_FATAL);
		}

		 
		if ((r = seek_pack(a)) < 0)
			return (r);
		zip->pack_stream_bytes_unconsumed =
		    (size_t)zip->pack_stream_inbytes_remaining;
		read_consume(a);

		 
		for (i = 0; i < 3; i++) {
			const struct _7z_coder *coder = scoder[i];

			if ((r = seek_pack(a)) < 0) {
				free(b[0]); free(b[1]); free(b[2]);
				return (r);
			}

			if (sunpack[i] == (uint64_t)-1)
				zip->folder_outbytes_remaining =
				    zip->pack_stream_inbytes_remaining;
			else
				zip->folder_outbytes_remaining = sunpack[i];

			r = init_decompression(a, zip, coder, NULL);
			if (r != ARCHIVE_OK) {
				free(b[0]); free(b[1]); free(b[2]);
				return (ARCHIVE_FATAL);
			}

			 
			b[i] = malloc((size_t)zip->folder_outbytes_remaining);
			if (b[i] == NULL) {
				free(b[0]); free(b[1]); free(b[2]);
				archive_set_error(&a->archive, ENOMEM,
				    "No memory for 7-Zip decompression");
				return (ARCHIVE_FATAL);
			}

			 
			while (zip->pack_stream_inbytes_remaining > 0) {
				r = (int)extract_pack_stream(a, 0);
				if (r < 0) {
					free(b[0]); free(b[1]); free(b[2]);
					return (r);
				}
				bytes = get_uncompressed_data(a, &buff,
				    zip->uncompressed_buffer_bytes_remaining,
				    0);
				if (bytes < 0) {
					free(b[0]); free(b[1]); free(b[2]);
					return ((int)bytes);
				}
				memcpy(b[i]+s[i], buff, bytes);
				s[i] += bytes;
				if (zip->pack_stream_bytes_unconsumed)
					read_consume(a);
			}
		}

		 
		for (i = 0; i < 3; i++) {
			zip->sub_stream_buff[i] = b[idx[i]];
			zip->sub_stream_size[i] = s[idx[i]];
			zip->sub_stream_bytes_remaining[i] = s[idx[i]];
		}

		 
		if (zip->tmp_stream_buff == NULL) {
			zip->tmp_stream_buff_size = 32 * 1024;
			zip->tmp_stream_buff =
			    malloc(zip->tmp_stream_buff_size);
			if (zip->tmp_stream_buff == NULL) {
				archive_set_error(&a->archive, ENOMEM,
				    "No memory for 7-Zip decompression");
				return (ARCHIVE_FATAL);
			}
		}
		zip->tmp_stream_bytes_avail = 0;
		zip->tmp_stream_bytes_remaining = 0;
		zip->odd_bcj_size = 0;
		zip->bcj2_outPos = 0;

		 
		zip->pack_stream_remaining = 1;
		zip->pack_stream_index = (unsigned)folder->packIndex;
		zip->folder_outbytes_remaining =
		    folder_uncompressed_size(folder);
		zip->uncompressed_buffer_bytes_remaining = 0;
	}

	 
	r = init_decompression(a, zip, coder1, coder2);
	if (r != ARCHIVE_OK)
		return (ARCHIVE_FATAL);
	return (ARCHIVE_OK);
}