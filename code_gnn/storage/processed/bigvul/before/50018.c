archive_read_has_encrypted_entries(struct archive *_a)
{
	struct archive_read *a = (struct archive_read *)_a;
	int format_supports_encryption = archive_read_format_capabilities(_a)
			& (ARCHIVE_READ_FORMAT_CAPS_ENCRYPT_DATA | ARCHIVE_READ_FORMAT_CAPS_ENCRYPT_METADATA);

	if (!_a || !format_supports_encryption) {
		 
		return ARCHIVE_READ_FORMAT_ENCRYPTION_UNSUPPORTED;
	}

	 
	if (a->format && a->format->has_encrypted_entries) {
		return (a->format->has_encrypted_entries)(a);
	}

	 
	return ARCHIVE_READ_FORMAT_ENCRYPTION_DONT_KNOW;
}
