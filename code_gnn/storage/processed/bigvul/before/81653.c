event_create(void)
{
	int evhdl = eventfd(0, EFD_CLOEXEC);
	int *ret;

	if (evhdl == -1) {
		 
		 
		return 0;
	}

	ret = (int *)mg_malloc(sizeof(int));
	if (ret) {
		*ret = evhdl;
	} else {
		(void)close(evhdl);
	}

	return (void *)ret;
}
