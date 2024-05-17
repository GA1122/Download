process_runtime_configs()
{
	int i, rval, fd;
	bool processed = false;

	for (i=0; i <= rArray.getlast(); i++) {
		processed = true;

		char* tmp_dir = temp_dir_path();
		ASSERT(tmp_dir);
		MyString tmp_file_tmpl = tmp_dir;
		free(tmp_dir);
		tmp_file_tmpl += "/cndrtmpXXXXXX";

		char* tmp_file = strdup(tmp_file_tmpl.Value());
		fd = condor_mkstemp( tmp_file );
		if (fd < 0) {
			dprintf( D_ALWAYS, "condor_mkstemp(%s) returned %d, '%s' (errno %d) in "
				 "process_dynamic_configs()\n", tmp_file, fd,
				 strerror(errno), errno );
			exit(1);
		}

		if (write(fd, rArray[i].config, strlen(rArray[i].config))
			!= (ssize_t)strlen(rArray[i].config)) {
			dprintf( D_ALWAYS, "write failed with errno %d in "
					 "process_dynamic_configs\n", errno );
			exit(1);
		}
		if (close(fd) < 0) {
			dprintf( D_ALWAYS, "close failed with errno %d in "
					 "process_dynamic_configs\n", errno );
			exit(1);
		}
		rval = Read_config( tmp_file, ConfigTab, TABLESIZE,
							EXPAND_LAZY, false, extra_info );
		if (rval < 0) {
			dprintf( D_ALWAYS, "Configuration Error Line %d "
					 "while reading %s, runtime config: %s\n",
					 ConfigLineNo, tmp_file, rArray[i].admin );
			exit(1);
		}
		unlink(tmp_file);
		free(tmp_file);
	}

	return (int)processed;
}
