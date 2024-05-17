event_signal(void *eventhdl)
{
	uint64_t u = 1;
	int evhdl, s;

	if (!eventhdl) {
		 
		return 0;
	}
	evhdl = *(int *)eventhdl;

	s = (int)write(evhdl, &u, sizeof(u));
	if (s != sizeof(u)) {
		 
		return 0;
	}
	return 1;
}
