static void FS_FreePak(pack_t *thepak)
{
	unzClose(thepak->handle);
	Z_Free(thepak->buildBuffer);
	Z_Free(thepak);
}