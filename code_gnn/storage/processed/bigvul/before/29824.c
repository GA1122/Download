cifs_parse_devname(const char *devname, struct smb_vol *vol)
{
	char *pos;
	const char *delims = "/\\";
	size_t len;

	 
	len = strspn(devname, delims);
	if (len != 2)
		return -EINVAL;

	 
	pos = strpbrk(devname + 2, delims);
	if (!pos)
		return -EINVAL;

	 
	++pos;

	 
	len = strcspn(pos, delims);

	 
	pos += len;
	vol->UNC = kstrndup(devname, pos - devname, GFP_KERNEL);
	if (!vol->UNC)
		return -ENOMEM;

	convert_delimiter(vol->UNC, '\\');

	 
	if (!*pos++ || !*pos)
		return 0;

	vol->prepath = kstrdup(pos, GFP_KERNEL);
	if (!vol->prepath)
		return -ENOMEM;

	return 0;
}
