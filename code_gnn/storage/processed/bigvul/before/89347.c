static int parse_label(char **c, struct pxe_menu *cfg)
{
	struct token t;
	int len;
	char *s = *c;
	struct pxe_label *label;
	int err;

	label = label_create();
	if (!label)
		return -ENOMEM;

	err = parse_sliteral(c, &label->name);
	if (err < 0) {
		printf("Expected label name: %.*s\n", (int)(*c - s), s);
		label_destroy(label);
		return -EINVAL;
	}

	list_add_tail(&label->list, &cfg->labels);

	while (1) {
		s = *c;
		get_token(c, &t, L_KEYWORD);

		err = 0;
		switch (t.type) {
		case T_MENU:
			err = parse_label_menu(c, cfg, label);
			break;

		case T_KERNEL:
		case T_LINUX:
			err = parse_label_kernel(c, label);
			break;

		case T_APPEND:
			err = parse_sliteral(c, &label->append);
			if (label->initrd)
				break;
			s = strstr(label->append, "initrd=");
			if (!s)
				break;
			s += 7;
			len = (int)(strchr(s, ' ') - s);
			label->initrd = malloc(len + 1);
			strncpy(label->initrd, s, len);
			label->initrd[len] = '\0';

			break;

		case T_INITRD:
			if (!label->initrd)
				err = parse_sliteral(c, &label->initrd);
			break;

		case T_FDT:
			if (!label->fdt)
				err = parse_sliteral(c, &label->fdt);
			break;

		case T_FDTDIR:
			if (!label->fdtdir)
				err = parse_sliteral(c, &label->fdtdir);
			break;

		case T_LOCALBOOT:
			label->localboot = 1;
			err = parse_integer(c, &label->localboot_val);
			break;

		case T_IPAPPEND:
			err = parse_integer(c, &label->ipappend);
			break;

		case T_EOL:
			break;

		default:
			 
			*c = s;
			return 1;
		}

		if (err < 0)
			return err;
	}
}
