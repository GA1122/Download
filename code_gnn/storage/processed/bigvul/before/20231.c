hugetlbfs_parse_options(char *options, struct hugetlbfs_config *pconfig)
{
	char *p, *rest;
	substring_t args[MAX_OPT_ARGS];
	int option;
	unsigned long long size = 0;
	enum { NO_SIZE, SIZE_STD, SIZE_PERCENT } setsize = NO_SIZE;

	if (!options)
		return 0;

	while ((p = strsep(&options, ",")) != NULL) {
		int token;
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_uid:
			if (match_int(&args[0], &option))
 				goto bad_val;
			pconfig->uid = option;
			break;

		case Opt_gid:
			if (match_int(&args[0], &option))
 				goto bad_val;
			pconfig->gid = option;
			break;

		case Opt_mode:
			if (match_octal(&args[0], &option))
 				goto bad_val;
			pconfig->mode = option & 01777U;
			break;

		case Opt_size: {
			 
			if (!isdigit(*args[0].from))
				goto bad_val;
			size = memparse(args[0].from, &rest);
			setsize = SIZE_STD;
			if (*rest == '%')
				setsize = SIZE_PERCENT;
			break;
		}

		case Opt_nr_inodes:
			 
			if (!isdigit(*args[0].from))
				goto bad_val;
			pconfig->nr_inodes = memparse(args[0].from, &rest);
			break;

		case Opt_pagesize: {
			unsigned long ps;
			ps = memparse(args[0].from, &rest);
			pconfig->hstate = size_to_hstate(ps);
			if (!pconfig->hstate) {
				printk(KERN_ERR
				"hugetlbfs: Unsupported page size %lu MB\n",
					ps >> 20);
				return -EINVAL;
			}
			break;
		}

		default:
			printk(KERN_ERR "hugetlbfs: Bad mount option: \"%s\"\n",
				 p);
			return -EINVAL;
			break;
		}
	}

	 
	if (setsize > NO_SIZE) {
		struct hstate *h = pconfig->hstate;
		if (setsize == SIZE_PERCENT) {
			size <<= huge_page_shift(h);
			size *= h->max_huge_pages;
			do_div(size, 100);
		}
		pconfig->nr_blocks = (size >> huge_page_shift(h));
	}

	return 0;

bad_val:
 	printk(KERN_ERR "hugetlbfs: Bad value '%s' for mount option '%s'\n",
	       args[0].from, p);
 	return -EINVAL;
}