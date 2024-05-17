static int handle_include(cmd_tbl_t *cmdtp, char **c, unsigned long base,
			  struct pxe_menu *cfg, int nest_level)
{
	char *include_path;
	char *s = *c;
	int err;
	char *buf;
	int ret;

	err = parse_sliteral(c, &include_path);

	if (err < 0) {
		printf("Expected include path: %.*s\n", (int)(*c - s), s);
		return err;
	}

	err = get_pxe_file(cmdtp, include_path, base);

	if (err < 0) {
		printf("Couldn't retrieve %s\n", include_path);
		return err;
	}

	buf = map_sysmem(base, 0);
	ret = parse_pxefile_top(cmdtp, buf, base, cfg, nest_level);
	unmap_sysmem(buf);

	return ret;
}
