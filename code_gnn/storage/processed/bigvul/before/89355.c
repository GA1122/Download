int zfs_print(const char *entry, const struct zfs_dirhook_info *data)
{
	printf("%s %s\n",
		   data->dir ? "<DIR> " : "		 ",
		   entry);
	return 0;  
}
