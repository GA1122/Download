static int parse_label_kernel(char **c, struct pxe_label *label)
{
	char *s;
	int err;

	err = parse_sliteral(c, &label->kernel);
	if (err < 0)
		return err;

	s = strstr(label->kernel, "#");
	if (!s)
		return 1;

	label->config = malloc(strlen(s) + 1);
	if (!label->config)
		return -ENOMEM;

	strcpy(label->config, s);
	*s = 0;

	return 1;
}
