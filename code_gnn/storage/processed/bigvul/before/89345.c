static void label_print(void *data)
{
	struct pxe_label *label = data;
	const char *c = label->menu ? label->menu : label->name;

	printf("%s:\t%s\n", label->num, c);
}
