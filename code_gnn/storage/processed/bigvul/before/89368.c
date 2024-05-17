static int spl_fit_get_image_node(const void *fit, int images,
				  const char *type, int index)
{
	const char *str;
	int err;
	int node;

	err = spl_fit_get_image_name(fit, images, type, index, &str);
	if (err)
		return err;

	debug("%s: '%s'\n", type, str);

	node = fdt_subnode_offset(fit, images, str);
	if (node < 0) {
		pr_err("cannot find image node '%s': %d\n", str, node);
		return -EINVAL;
	}

	return node;
}
