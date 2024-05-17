static struct menu *pxe_menu_to_menu(struct pxe_menu *cfg)
{
	struct pxe_label *label;
	struct list_head *pos;
	struct menu *m;
	int err;
	int i = 1;
	char *default_num = NULL;

	 
	m = menu_create(cfg->title, DIV_ROUND_UP(cfg->timeout, 10),
			cfg->prompt, label_print, NULL, NULL);

	if (!m)
		return NULL;

	list_for_each(pos, &cfg->labels) {
		label = list_entry(pos, struct pxe_label, list);

		sprintf(label->num, "%d", i++);
		if (menu_item_add(m, label->num, label) != 1) {
			menu_destroy(m);
			return NULL;
		}
		if (cfg->default_label &&
		    (strcmp(label->name, cfg->default_label) == 0))
			default_num = label->num;
	}

	 
	if (default_num) {
		err = menu_default_set(m, default_num);
		if (err != 1) {
			if (err != -ENOENT) {
				menu_destroy(m);
				return NULL;
			}

			printf("Missing default: %s\n", cfg->default_label);
		}
	}

	return m;
}
