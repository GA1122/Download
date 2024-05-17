read_decryption_header(struct archive_read *a)
{
	struct zip *zip = (struct zip *)(a->format->data);
	const char *p;
	unsigned int remaining_size;
	unsigned int ts;

	 
	p = __archive_read_ahead(a, 2, NULL);
	if (p == NULL)
		goto truncated;
	ts = zip->iv_size;
	zip->iv_size = archive_le16dec(p);
	__archive_read_consume(a, 2);
	if (ts < zip->iv_size) {
		free(zip->iv);
		zip->iv = NULL;
	}
	p = __archive_read_ahead(a, zip->iv_size, NULL);
	if (p == NULL)
		goto truncated;
	if (zip->iv == NULL) {
		zip->iv = malloc(zip->iv_size);
		if (zip->iv == NULL)
			goto nomem;
	}
	memcpy(zip->iv, p, zip->iv_size);
	__archive_read_consume(a, zip->iv_size);

	 
	p = __archive_read_ahead(a, 14, NULL);
	if (p == NULL)
		goto truncated;
	remaining_size = archive_le32dec(p);
	if (remaining_size < 16 || remaining_size > (1 << 18))
		goto corrupted;

	 
	if (archive_le16dec(p+4) != 3) {
		archive_set_error(&a->archive,
		    ARCHIVE_ERRNO_FILE_FORMAT,
		    "Unsupported encryption format version: %u",
		    archive_le16dec(p+4));
		return (ARCHIVE_FAILED);
	}

	 
	zip->alg_id = archive_le16dec(p+6);
	switch (zip->alg_id) {
	case 0x6601: 
	case 0x6602: 
	case 0x6603: 
	case 0x6609: 
	case 0x660E: 
	case 0x660F: 
	case 0x6610: 
	case 0x6702: 
	case 0x6720: 
	case 0x6721: 
	case 0x6801: 
		 
		break;
	default:
		archive_set_error(&a->archive,
		    ARCHIVE_ERRNO_FILE_FORMAT,
		    "Unknown encryption algorithm: %u", zip->alg_id);
		return (ARCHIVE_FAILED);
	}

	 
	zip->bit_len = archive_le16dec(p+8);

	 
	zip->flags = archive_le16dec(p+10);
	switch (zip->flags & 0xf000) {
	case 0x0001:  
	case 0x0002:  
	case 0x0003:  
		break;
	default:
		archive_set_error(&a->archive,
		    ARCHIVE_ERRNO_FILE_FORMAT,
		    "Unknown encryption flag: %u", zip->flags);
		return (ARCHIVE_FAILED);
	}
	if ((zip->flags & 0xf000) == 0 ||
	    (zip->flags & 0xf000) == 0x4000) {
		archive_set_error(&a->archive,
		    ARCHIVE_ERRNO_FILE_FORMAT,
		    "Unknown encryption flag: %u", zip->flags);
		return (ARCHIVE_FAILED);
	}

	 
	ts = zip->erd_size;
	zip->erd_size = archive_le16dec(p+12);
	__archive_read_consume(a, 14);
	if ((zip->erd_size & 0xf) != 0 ||
	    (zip->erd_size + 16) > remaining_size ||
	    (zip->erd_size + 16) < zip->erd_size)
		goto corrupted;

	if (ts < zip->erd_size) {
		free(zip->erd);
		zip->erd = NULL;
	}
	p = __archive_read_ahead(a, zip->erd_size, NULL);
	if (p == NULL)
		goto truncated;
	if (zip->erd == NULL) {
		zip->erd = malloc(zip->erd_size);
		if (zip->erd == NULL)
			goto nomem;
	}
	memcpy(zip->erd, p, zip->erd_size);
	__archive_read_consume(a, zip->erd_size);

	 
	p = __archive_read_ahead(a, 4, NULL);
	if (p == NULL)
		goto truncated;
	 
	if (archive_le32dec(p) != 0)
		goto corrupted;
	__archive_read_consume(a, 4);

	 
	p = __archive_read_ahead(a, 2, NULL);
	if (p == NULL)
		goto truncated;
	ts = zip->v_size;
	zip->v_size = archive_le16dec(p);
	__archive_read_consume(a, 2);
	if ((zip->v_size & 0x0f) != 0 ||
	    (zip->erd_size + zip->v_size + 16) > remaining_size ||
	    (zip->erd_size + zip->v_size + 16) < (zip->erd_size + zip->v_size))
		goto corrupted;
	if (ts < zip->v_size) {
		free(zip->v_data);
		zip->v_data = NULL;
	}
	p = __archive_read_ahead(a, zip->v_size, NULL);
	if (p == NULL)
		goto truncated;
	if (zip->v_data == NULL) {
		zip->v_data = malloc(zip->v_size);
		if (zip->v_data == NULL)
			goto nomem;
	}
	memcpy(zip->v_data, p, zip->v_size);
	__archive_read_consume(a, zip->v_size);

	p = __archive_read_ahead(a, 4, NULL);
	if (p == NULL)
		goto truncated;
	zip->v_crc32 = archive_le32dec(p);
	__archive_read_consume(a, 4);

	 
	archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
	    "Encrypted file is unsupported");
	return (ARCHIVE_FAILED);
truncated:
	archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
	    "Truncated ZIP file data");
	return (ARCHIVE_FATAL);
corrupted:
	archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
	    "Corrupted ZIP file data");
	return (ARCHIVE_FATAL);
nomem:
	archive_set_error(&a->archive, ENOMEM,
	    "No memory for ZIP decryption");
	return (ARCHIVE_FATAL);
}
