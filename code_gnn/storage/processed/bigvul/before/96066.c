long FS_filelength(fileHandle_t f)
{
	FILE	*h;

	h = FS_FileForHandle(f);
	
	if(h == NULL)
		return -1;
	else
		return FS_fplength(h);
}
