static void label_destroy(struct pxe_label *label)
{
	if (label->name)
		free(label->name);

	if (label->kernel)
		free(label->kernel);

	if (label->config)
		free(label->config);

	if (label->append)
		free(label->append);

	if (label->initrd)
		free(label->initrd);

	if (label->fdt)
		free(label->fdt);

	if (label->fdtdir)
		free(label->fdtdir);

	free(label);
}
