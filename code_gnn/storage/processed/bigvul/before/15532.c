mux_client_forwards(int fd, int cancel_flag)
{
	int i, ret = 0;

	debug3("%s: %s forwardings: %d local, %d remote", __func__,
	    cancel_flag ? "cancel" : "request",
	    options.num_local_forwards, options.num_remote_forwards);

	 
	for (i = 0; i < options.num_local_forwards; i++) {
		if (mux_client_forward(fd, cancel_flag,
		    options.local_forwards[i].connect_port == 0 ?
		    MUX_FWD_DYNAMIC : MUX_FWD_LOCAL,
		    options.local_forwards + i) != 0)
			ret = -1;
	}
	for (i = 0; i < options.num_remote_forwards; i++) {
		if (mux_client_forward(fd, cancel_flag, MUX_FWD_REMOTE,
		    options.remote_forwards + i) != 0)
			ret = -1;
	}
	return ret;
}
