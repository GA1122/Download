static void print_time(void) {
	if (start_timestamp) {
		unsigned long long end_timestamp = getticks();
		usleep(1000);
		unsigned long long onems = getticks() - end_timestamp;
		if (onems) {
			fmessage("Child process initialized in %.02f ms\n",
				(float) (end_timestamp - start_timestamp) / (float) onems);
			return;
		}
	}

	fmessage("Child process initialized\n");
}