bool dir_exists(const char *path)
{
	struct stat sb;
	int ret;

	ret = stat(path, &sb);
	if (ret < 0)
		return false;
	return S_ISDIR(sb.st_mode);
}
