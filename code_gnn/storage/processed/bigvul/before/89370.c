static int spl_fit_record_loadable(const void *fit, int images, int index,
				   void *blob, struct spl_image_info *image)
{
	int ret = 0;
#if !CONFIG_IS_ENABLED(FIT_IMAGE_TINY)
	const char *name;
	int node;

	ret = spl_fit_get_image_name(fit, images, "loadables",
				     index, &name);
	if (ret < 0)
		return ret;

	node = spl_fit_get_image_node(fit, images, "loadables", index);

	ret = fdt_record_loadable(blob, index, name, image->load_addr,
				  image->size, image->entry_point,
				  fdt_getprop(fit, node, "type", NULL),
				  fdt_getprop(fit, node, "os", NULL));
#endif
	return ret;
}
