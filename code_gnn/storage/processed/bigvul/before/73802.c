static UPNP_INLINE int get_alias(
	 
	const char *request_file,
	 
	struct xml_alias_t *alias,
	 
	UpnpFileInfo *info)
{
	int cmp = strcmp(alias->name.buf, request_file);
	if (cmp == 0) {
		UpnpFileInfo_set_FileLength(info, (off_t)alias->doc.length);
		UpnpFileInfo_set_IsDirectory(info, FALSE);
		UpnpFileInfo_set_IsReadable(info, TRUE);
		UpnpFileInfo_set_LastModified(info, alias->last_modified);
	}

	return cmp == 0;
}
