event_wait(void *eventhdl)
{
	uint64_t u;
	int evhdl, s;

	if (!eventhdl) {
		 
		return 0;
	}
	evhdl = *(int *)eventhdl;

	s = (int)read(evhdl, &u, sizeof(u));
	if (s != sizeof(u)) {
		 
		return 0;
	}
	(void)u;  
	return 1;
}
