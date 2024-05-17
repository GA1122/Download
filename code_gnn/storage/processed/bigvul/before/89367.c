static int spl_fit_get_image_name(const void *fit, int images,
				  const char *type, int index,
				  const char **outname)
{
	struct udevice *board;
	const char *name, *str;
	__maybe_unused int node;
	int conf_node;
	int len, i;
	bool found = true;

	conf_node = fit_find_config_node(fit);
	if (conf_node < 0) {
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
		printf("No matching DT out of these options:\n");
		for (node = fdt_first_subnode(fit, conf_node);
		     node >= 0;
		     node = fdt_next_subnode(fit, node)) {
			name = fdt_getprop(fit, node, "description", &len);
			printf("   %s\n", name);
		}
#endif
		return conf_node;
	}

	name = fdt_getprop(fit, conf_node, type, &len);
	if (!name) {
		debug("cannot find property '%s': %d\n", type, len);
		return -EINVAL;
	}

	str = name;
	for (i = 0; i < index; i++) {
		str = strchr(str, '\0') + 1;
		if (!str || (str - name >= len)) {
			found = false;
			break;
		}
	}

	if (!found && !board_get(&board)) {
		int rc;
		 
		rc = board_get_fit_loadable(board, index - i - 1, type, &str);
		if (rc && rc != -ENOENT)
			return rc;

		if (!rc) {
			 
			int node;
			int images = fdt_path_offset(fit, FIT_IMAGES_PATH);

			node = find_node_from_desc(fit, images, str);
			if (node > 0)
				str = fdt_get_name(fit, node, NULL);

			found = true;
		}
	}

	if (!found) {
		debug("no string for index %d\n", index);
		return -E2BIG;
	}

	*outname = str;
	return 0;
}
