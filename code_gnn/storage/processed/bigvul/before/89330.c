void destroy_pxe_menu(struct pxe_menu *cfg)
{
	struct list_head *pos, *n;
	struct pxe_label *label;

	if (cfg->title)
		free(cfg->title);

	if (cfg->default_label)
		free(cfg->default_label);

	list_for_each_safe(pos, n, &cfg->labels) {
		label = list_entry(pos, struct pxe_label, list);

		label_destroy(label);
	}

	free(cfg);
}
