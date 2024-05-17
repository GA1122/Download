void setupexport(CLIENT* client) {
	int i;
	off_t laststartoff = 0, lastsize = 0;
	int multifile = (client->server->flags & F_MULTIFILE);

	client->export = g_array_new(TRUE, TRUE, sizeof(FILE_INFO));

	 
	for(i=0; ; i++) {
		FILE_INFO fi;
		gchar *tmpname;
		gchar* error_string;
		mode_t mode = (client->server->flags & F_READONLY) ? O_RDONLY : O_RDWR;

		if(multifile) {
			tmpname=g_strdup_printf("%s.%d", client->exportname, i);
		} else {
			tmpname=g_strdup(client->exportname);
		}
		DEBUG2( "Opening %s\n", tmpname );
		fi.fhandle = open(tmpname, mode);
		if(fi.fhandle == -1 && mode == O_RDWR) {
			 
			fi.fhandle = open(tmpname, O_RDONLY);
			if(fi.fhandle != -1) {
				 
				if(!(client->server->flags & F_COPYONWRITE)) {
					client->server->flags |= F_AUTOREADONLY;
					client->server->flags |= F_READONLY;
				}
			}
		}
		if(fi.fhandle == -1) {
			if(multifile && i>0)
				break;
			error_string=g_strdup_printf(
				"Could not open exported file %s: %%m",
				tmpname);
			err(error_string);
		}
		fi.startoff = laststartoff + lastsize;
		g_array_append_val(client->export, fi);
		g_free(tmpname);

		 
		laststartoff = fi.startoff;
		lastsize = size_autodetect(fi.fhandle);

		if(!multifile)
			break;
	}

	 
	client->exportsize = laststartoff + lastsize;

	 
	if(client->server->expected_size) {
		 
		if(client->server->expected_size > client->exportsize) {
			err("Size of exported file is too big\n");
		}

		client->exportsize = client->server->expected_size;
	}

	msg3(LOG_INFO, "Size of exported file/device is %llu", (unsigned long long)client->exportsize);
	if(multifile) {
		msg3(LOG_INFO, "Total number of files: %d", i);
	}
}
