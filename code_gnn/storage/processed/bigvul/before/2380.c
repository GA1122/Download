static void convert_sbuf(vfs_handle_struct *handle, const char *fname,
			 SMB_STRUCT_STAT *sbuf)
{
	struct shadow_copy2_config *config;

	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
				return);

	if (config->fixinodes) {
		 
		TDB_DATA key = { .dptr = discard_const_p(uint8_t, fname),
				 .dsize = strlen(fname) };
		uint32_t shash;

		shash = tdb_jenkins_hash(&key) & 0xFF000000;
		if (shash == 0) {
			shash = 1;
		}
		sbuf->st_ex_ino ^= shash;
	}
}
