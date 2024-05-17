static void boot_unattempted_labels(cmd_tbl_t *cmdtp, struct pxe_menu *cfg)
{
	struct list_head *pos;
	struct pxe_label *label;

	list_for_each(pos, &cfg->labels) {
		label = list_entry(pos, struct pxe_label, list);

		if (!label->attempted)
			label_boot(cmdtp, label);
	}
}
