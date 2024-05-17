int main(int argc, char *argv[])
{
	char *dest = "squashfs-root";
	int i, stat_sys = FALSE, version = FALSE;
	int n;
	struct pathnames *paths = NULL;
	struct pathname *path = NULL;
	int fragment_buffer_size = FRAGMENT_BUFFER_DEFAULT;
	int data_buffer_size = DATA_BUFFER_DEFAULT;

	pthread_mutex_init(&screen_mutex, NULL);
	root_process = geteuid() == 0;
	if(root_process)
		umask(0);
	
	for(i = 1; i < argc; i++) {
		if(*argv[i] != '-')
			break;
		if(strcmp(argv[i], "-version") == 0 ||
				strcmp(argv[i], "-v") == 0) {
			VERSION();
			version = TRUE;
		} else if(strcmp(argv[i], "-info") == 0 ||
				strcmp(argv[i], "-i") == 0)
			info = TRUE;
		else if(strcmp(argv[i], "-ls") == 0 ||
				strcmp(argv[i], "-l") == 0)
			lsonly = TRUE;
		else if(strcmp(argv[i], "-no-progress") == 0 ||
				strcmp(argv[i], "-n") == 0)
			progress = FALSE;
		else if(strcmp(argv[i], "-no-xattrs") == 0 ||
				strcmp(argv[i], "-no") == 0)
			no_xattrs = TRUE;
		else if(strcmp(argv[i], "-xattrs") == 0 ||
				strcmp(argv[i], "-x") == 0)
			no_xattrs = FALSE;
		else if(strcmp(argv[i], "-user-xattrs") == 0 ||
				strcmp(argv[i], "-u") == 0) {
			user_xattrs = TRUE;
			no_xattrs = FALSE;
		} else if(strcmp(argv[i], "-dest") == 0 ||
				strcmp(argv[i], "-d") == 0) {
			if(++i == argc) {
				fprintf(stderr, "%s: -dest missing filename\n",
					argv[0]);
				exit(1);
			}
			dest = argv[i];
        		} else if (strcmp(argv[i], "-offset") == 0 ||
				strcmp(argv[i], "-o") == 0) {
			if(++i == argc) {
				fprintf(stderr, "%s: -offset missing argument\n",
					argv[0]);
				exit(1);
			}
			squashfs_start_offset = (off_t)atol(argv[i]);
		} else if(strcmp(argv[i], "-processors") == 0 ||
				strcmp(argv[i], "-p") == 0) {
			if((++i == argc) || 
					!parse_number(argv[i],
						&processors)) {
				ERROR("%s: -processors missing or invalid "
					"processor number\n", argv[0]);
				exit(1);
			}
			if(processors < 1) {
				ERROR("%s: -processors should be 1 or larger\n",
					argv[0]);
				exit(1);
			}
		} else if(strcmp(argv[i], "-data-queue") == 0 ||
					 strcmp(argv[i], "-da") == 0) {
			if((++i == argc) ||
					!parse_number(argv[i],
						&data_buffer_size)) {
				ERROR("%s: -data-queue missing or invalid "
					"queue size\n", argv[0]);
				exit(1);
			}
			if(data_buffer_size < 1) {
				ERROR("%s: -data-queue should be 1 Mbyte or "
					"larger\n", argv[0]);
				exit(1);
			}
		} else if(strcmp(argv[i], "-frag-queue") == 0 ||
					strcmp(argv[i], "-fr") == 0) {
			if((++i == argc) ||
					!parse_number(argv[i],
						&fragment_buffer_size)) {
				ERROR("%s: -frag-queue missing or invalid "
					"queue size\n", argv[0]);
				exit(1);
			}
			if(fragment_buffer_size < 1) {
				ERROR("%s: -frag-queue should be 1 Mbyte or "
					"larger\n", argv[0]);
				exit(1);
			}
		} else if(strcmp(argv[i], "-force") == 0 ||
				strcmp(argv[i], "-f") == 0)
			force = TRUE;
		else if(strcmp(argv[i], "-stat") == 0 ||
				strcmp(argv[i], "-s") == 0)
			stat_sys = TRUE;
		else if(strcmp(argv[i], "-lls") == 0 ||
				strcmp(argv[i], "-ll") == 0) {
			lsonly = TRUE;
			short_ls = FALSE;
		} else if(strcmp(argv[i], "-linfo") == 0 ||
				strcmp(argv[i], "-li") == 0) {
			info = TRUE;
			short_ls = FALSE;
		} else if(strcmp(argv[i], "-ef") == 0 ||
				strcmp(argv[i], "-e") == 0) {
			if(++i == argc) {
				fprintf(stderr, "%s: -ef missing filename\n",
					argv[0]);
				exit(1);
			}
			path = process_extract_files(path, argv[i]);
		} else if(strcmp(argv[i], "-regex") == 0 ||
				strcmp(argv[i], "-r") == 0)
			use_regex = TRUE;
		else
			goto options;
	}

	if(lsonly || info)
		progress = FALSE;

#ifdef SQUASHFS_TRACE
	 
	progress = FALSE;
#endif

	if(i == argc) {
		if(!version) {
options:
			ERROR("SYNTAX: %s [options] filesystem [directories or "
				"files to extract]\n", argv[0]);
			ERROR("\t-v[ersion]\t\tprint version, licence and "
				"copyright information\n");
			ERROR("\t-d[est] <pathname>\tunsquash to <pathname>, "
				"default \"squashfs-root\"\n");
			ERROR("\t-o[ffset] <bytes>\tskip <bytes> at start of input file, "
				"default \"0\"\n");
			ERROR("\t-n[o-progress]\t\tdon't display the progress "
				"bar\n");
			ERROR("\t-no[-xattrs]\t\tdon't extract xattrs in file system"
				NOXOPT_STR"\n");
			ERROR("\t-x[attrs]\t\textract xattrs in file system"
				XOPT_STR "\n");
			ERROR("\t-u[ser-xattrs]\t\tonly extract user xattrs in "
				"file system.\n\t\t\t\tEnables extracting "
				"xattrs\n");
			ERROR("\t-p[rocessors] <number>\tuse <number> "
				"processors.  By default will use\n");
			ERROR("\t\t\t\tnumber of processors available\n");
			ERROR("\t-i[nfo]\t\t\tprint files as they are "
				"unsquashed\n");
			ERROR("\t-li[nfo]\t\tprint files as they are "
				"unsquashed with file\n");
			ERROR("\t\t\t\tattributes (like ls -l output)\n");
			ERROR("\t-l[s]\t\t\tlist filesystem, but don't unsquash"
				"\n");
			ERROR("\t-ll[s]\t\t\tlist filesystem with file "
				"attributes (like\n");
			ERROR("\t\t\t\tls -l output), but don't unsquash\n");
			ERROR("\t-f[orce]\t\tif file already exists then "
				"overwrite\n");
			ERROR("\t-s[tat]\t\t\tdisplay filesystem superblock "
				"information\n");
			ERROR("\t-e[f] <extract file>\tlist of directories or "
				"files to extract.\n\t\t\t\tOne per line\n");
			ERROR("\t-da[ta-queue] <size>\tSet data queue to "
				"<size> Mbytes.  Default %d\n\t\t\t\tMbytes\n",
				DATA_BUFFER_DEFAULT);
			ERROR("\t-fr[ag-queue] <size>\tSet fragment queue to "
				"<size> Mbytes.  Default\n\t\t\t\t%d Mbytes\n",
				FRAGMENT_BUFFER_DEFAULT);
			ERROR("\t-r[egex]\t\ttreat extract names as POSIX "
				"regular expressions\n");
			ERROR("\t\t\t\trather than use the default shell "
				"wildcard\n\t\t\t\texpansion (globbing)\n");
			ERROR("\nDecompressors available:\n");
			display_compressors("", "");
		}
		exit(1);
	}

	for(n = i + 1; n < argc; n++)
		path = add_path(path, argv[n], argv[n]);

	if((fd = open(argv[i], O_RDONLY)) == -1) {
		ERROR("Could not open %s, because %s\n", argv[i],
			strerror(errno));
		exit(1);
	}

	if(read_super(argv[i]) == FALSE)
		exit(1);

	if(stat_sys) {
		squashfs_stat(argv[i]);
		exit(0);
	}

	if(!check_compression(comp))
		exit(1);

	block_size = sBlk.s.block_size;
	block_log = sBlk.s.block_log;

	 
	if(block_size > SQUASHFS_FILE_MAX_SIZE ||
					block_log > SQUASHFS_FILE_MAX_LOG)
		EXIT_UNSQUASH("Block size or block_log too large."
			"  File system is corrupt.\n");

	 
	if(block_size != (1 << block_log))
		EXIT_UNSQUASH("Block size and block_log do not match."
			"  File system is corrupt.\n");

	 
	if(shift_overflow(fragment_buffer_size, 20 - block_log))
		EXIT_UNSQUASH("Fragment queue size is too large\n");
	else
		fragment_buffer_size <<= 20 - block_log;

	if(shift_overflow(data_buffer_size, 20 - block_log))
		EXIT_UNSQUASH("Data queue size is too large\n");
	else
		data_buffer_size <<= 20 - block_log;

	initialise_threads(fragment_buffer_size, data_buffer_size);

	created_inode = malloc(sBlk.s.inodes * sizeof(char *));
	if(created_inode == NULL)
		EXIT_UNSQUASH("failed to allocate created_inode\n");

	memset(created_inode, 0, sBlk.s.inodes * sizeof(char *));

	if(s_ops.read_filesystem_tables() == FALSE)
		EXIT_UNSQUASH("failed to read file system tables\n");

	if(path) {
		paths = init_subdir();
		paths = add_subdir(paths, path);
	}

	pre_scan(dest, SQUASHFS_INODE_BLK(sBlk.s.root_inode),
		SQUASHFS_INODE_OFFSET(sBlk.s.root_inode), paths);

	memset(created_inode, 0, sBlk.s.inodes * sizeof(char *));
	inode_number = 1;

	printf("%d inodes (%d blocks) to write\n\n", total_inodes,
		total_inodes - total_files + total_blocks);

	enable_progress_bar();

	dir_scan(dest, SQUASHFS_INODE_BLK(sBlk.s.root_inode),
		SQUASHFS_INODE_OFFSET(sBlk.s.root_inode), paths);

	queue_put(to_writer, NULL);
	queue_get(from_writer);

	disable_progress_bar();

	if(!lsonly) {
		printf("\n");
		printf("created %d files\n", file_count);
		printf("created %d directories\n", dir_count);
		printf("created %d symlinks\n", sym_count);
		printf("created %d devices\n", dev_count);
		printf("created %d fifos\n", fifo_count);
	}

	if (FAILED)
		return 1;

	return 0;
}