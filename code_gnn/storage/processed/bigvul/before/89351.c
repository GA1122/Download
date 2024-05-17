struct pxe_menu *parse_pxefile(cmd_tbl_t *cmdtp, unsigned long menucfg)
{
	struct pxe_menu *cfg;
	char *buf;
	int r;

	cfg = malloc(sizeof(struct pxe_menu));

	if (!cfg)
		return NULL;

	memset(cfg, 0, sizeof(struct pxe_menu));

	INIT_LIST_HEAD(&cfg->labels);

	buf = map_sysmem(menucfg, 0);
	r = parse_pxefile_top(cmdtp, buf, menucfg, cfg, 1);
	unmap_sysmem(buf);

	if (r < 0) {
		destroy_pxe_menu(cfg);
		return NULL;
	}

	return cfg;
}
