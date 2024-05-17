int main(int argc, char **argv)
{
	blkid_cache cache = NULL;
	char **devices = NULL;
	char *show[128] = { NULL, };
	char *search_type = NULL, *search_value = NULL;
	char *read = NULL;
	int fltr_usage = 0;
	char **fltr_type = NULL;
	int fltr_flag = BLKID_FLTR_ONLYIN;
	unsigned int numdev = 0, numtag = 0;
	int version = 0;
	int err = BLKID_EXIT_OTHER;
	unsigned int i;
	int output_format = 0;
	int lookup = 0, gc = 0, lowprobe = 0, eval = 0;
	int c;
	uintmax_t offset = 0, size = 0;

	static const ul_excl_t excl[] = {        
		{ 'n','u' },
		{ 0 }
	};
	int excl_st[ARRAY_SIZE(excl)] = UL_EXCL_STATUS_INIT;

	show[0] = NULL;
	atexit(close_stdout);

	while ((c = getopt (argc, argv,
			    "c:df:ghilL:n:ko:O:ps:S:t:u:U:w:Vv")) != EOF) {

		err_exclusive_options(c, NULL, excl, excl_st);

		switch (c) {
		case 'c':
			if (optarg && !*optarg)
				read = NULL;
			else
				read = optarg;
			break;
		case 'd':
			raw_chars = 1;
			break;
		case 'L':
			eval++;
			search_value = xstrdup(optarg);
			search_type = xstrdup("LABEL");
			break;
		case 'n':
			fltr_type = list_to_types(optarg, &fltr_flag);
			break;
		case 'u':
			fltr_usage = list_to_usage(optarg, &fltr_flag);
			break;
		case 'U':
			eval++;
			search_value = xstrdup(optarg);
			search_type = xstrdup("UUID");
			break;
		case 'i':
			lowprobe |= LOWPROBE_TOPOLOGY;
			break;
		case 'l':
			lookup++;
			break;
		case 'g':
			gc = 1;
			break;
		case 'k':
		{
			size_t idx = 0;
			const char *name = NULL;

			while (blkid_superblocks_get_name(idx++, &name, NULL) == 0)
				printf("%s\n", name);
			exit(EXIT_SUCCESS);
		}
		case 'o':
			if (!strcmp(optarg, "value"))
				output_format = OUTPUT_VALUE_ONLY;
			else if (!strcmp(optarg, "device"))
				output_format = OUTPUT_DEVICE_ONLY;
			else if (!strcmp(optarg, "list"))
				output_format = OUTPUT_PRETTY_LIST;	 
			else if (!strcmp(optarg, "udev"))
				output_format = OUTPUT_UDEV_LIST;
			else if (!strcmp(optarg, "export"))
				output_format = OUTPUT_EXPORT_LIST;
			else if (!strcmp(optarg, "full"))
				output_format = 0;
			else {
				fprintf(stderr, "Invalid output format %s. "
					"Choose from value,\n\t"
					"device, list, udev or full\n", optarg);
				exit(BLKID_EXIT_OTHER);
			}
			break;
		case 'O':
			offset = strtosize_or_err(optarg, "invalid offset argument");
			break;
		case 'p':
			lowprobe |= LOWPROBE_SUPERBLOCKS;
			break;
		case 's':
			if (numtag + 1 >= sizeof(show) / sizeof(*show)) {
				fprintf(stderr, "Too many tags specified\n");
				usage(err);
			}
			show[numtag++] = optarg;
			show[numtag] = NULL;
			break;
		case 'S':
			size = strtosize_or_err(optarg, "invalid size argument");
			break;
		case 't':
			if (search_type) {
				fprintf(stderr, "Can only search for "
						"one NAME=value pair\n");
				usage(err);
			}
			if (blkid_parse_tag_string(optarg,
						   &search_type,
						   &search_value)) {
				fprintf(stderr, "-t needs NAME=value pair\n");
				usage(err);
			}
			break;
		case 'V':
		case 'v':
			version = 1;
			break;
		case 'w':
			 
			break;
		case 'h':
			err = 0;
			 
		default:
			usage(err);
		}
	}


	 
	if (optind < argc) {
		devices = xcalloc(argc - optind, sizeof(char *));
		while (optind < argc)
			devices[numdev++] = argv[optind++];
	}

	if (version) {
		print_version(stdout);
		goto exit;
	}

	 
	if (lookup && output_format == OUTPUT_DEVICE_ONLY && search_type &&
	    (!strcmp(search_type, "LABEL") || !strcmp(search_type, "UUID"))) {
		eval++;
		lookup = 0;
	}

	if (!lowprobe && !eval && blkid_get_cache(&cache, read) < 0)
		goto exit;

	if (gc) {
		blkid_gc_cache(cache);
		err = 0;
		goto exit;
	}
	err = BLKID_EXIT_NOTFOUND;

	if (eval == 0 && (output_format & OUTPUT_PRETTY_LIST)) {
		if (lowprobe) {
			fprintf(stderr, "The low-level probing mode does not "
					"support 'list' output format\n");
			exit(BLKID_EXIT_OTHER);
		}
		pretty_print_dev(NULL);
	}

	if (lowprobe) {
		 
		blkid_probe pr;

		if (!numdev) {
			fprintf(stderr, "The low-level probing mode "
					"requires a device\n");
			exit(BLKID_EXIT_OTHER);
		}

		 
		if (!output_format  && (lowprobe & LOWPROBE_TOPOLOGY))
			output_format = OUTPUT_EXPORT_LIST;

		pr = blkid_new_probe();
		if (!pr)
			goto exit;

		if (lowprobe & LOWPROBE_SUPERBLOCKS) {
			blkid_probe_set_superblocks_flags(pr,
				BLKID_SUBLKS_LABEL | BLKID_SUBLKS_UUID |
				BLKID_SUBLKS_TYPE | BLKID_SUBLKS_SECTYPE |
				BLKID_SUBLKS_USAGE | BLKID_SUBLKS_VERSION);

			if (fltr_usage && blkid_probe_filter_superblocks_usage(
						pr, fltr_flag, fltr_usage))
				goto exit;

			else if (fltr_type && blkid_probe_filter_superblocks_type(
						pr, fltr_flag, fltr_type))
				goto exit;
		}

		for (i = 0; i < numdev; i++) {
			err = lowprobe_device(pr, devices[i], lowprobe, show,
					output_format,
					(blkid_loff_t) offset,
					(blkid_loff_t) size);
			if (err)
				break;
		}
		blkid_free_probe(pr);
	} else if (eval) {
		 
		char *res = blkid_evaluate_tag(search_type, search_value, NULL);
		if (res) {
			err = 0;
			printf("%s\n", res);
		}
	} else if (lookup) {
		 
		blkid_dev dev;

		if (!search_type) {
			fprintf(stderr, "The lookup option requires a "
				"search type specified using -t\n");
			exit(BLKID_EXIT_OTHER);
		}
		 
		for (i = 0; i < numdev; i++)
			blkid_get_dev(cache, devices[i], BLKID_DEV_NORMAL);

		if ((dev = blkid_find_dev_with_tag(cache, search_type,
						   search_value))) {
			print_tags(dev, show, output_format);
			err = 0;
		}
	 
	} else if (!numdev) {
		blkid_dev_iterate	iter;
		blkid_dev		dev;

		blkid_probe_all(cache);

		iter = blkid_dev_iterate_begin(cache);
		blkid_dev_set_search(iter, search_type, search_value);
		while (blkid_dev_next(iter, &dev) == 0) {
			dev = blkid_verify(cache, dev);
			if (!dev)
				continue;
			print_tags(dev, show, output_format);
			err = 0;
		}
		blkid_dev_iterate_end(iter);
	 
	} else for (i = 0; i < numdev; i++) {
		blkid_dev dev = blkid_get_dev(cache, devices[i],
						  BLKID_DEV_NORMAL);

		if (dev) {
			if (search_type &&
			    !blkid_dev_has_tag(dev, search_type,
					       search_value))
				continue;
			print_tags(dev, show, output_format);
			err = 0;
		}
	}

exit:
	free(search_type);
	free(search_value);
	free_types_list(fltr_type);
	if (!lowprobe && !eval)
		blkid_put_cache(cache);
	free(devices);
	return err;
}