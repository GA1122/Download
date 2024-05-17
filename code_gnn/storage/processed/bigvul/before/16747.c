static const char *humanly_readable_name(struct module_entry *m)
{
	 
	return m->probed_name ? m->probed_name : m->modname;
}
