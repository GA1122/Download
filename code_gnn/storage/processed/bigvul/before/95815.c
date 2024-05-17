long FS_ReadFile(const char *qpath, void **buffer)
{
	return FS_ReadFileDir(qpath, NULL, qfalse, buffer);
}
