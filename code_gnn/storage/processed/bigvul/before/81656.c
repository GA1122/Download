event_destroy(void *eventhdl)
{
	int evhdl;

	if (!eventhdl) {
		 
		return;
	}
	evhdl = *(int *)eventhdl;

	close(evhdl);
	mg_free(eventhdl);
}
