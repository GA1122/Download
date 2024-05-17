mux_exit_message(Channel *c, int exitval)
{
	Buffer m;
	Channel *mux_chan;

	debug3("%s: channel %d: exit message, exitval %d", __func__, c->self,
	    exitval);

	if ((mux_chan = channel_by_id(c->ctl_chan)) == NULL)
		fatal("%s: channel %d missing mux channel %d",
		    __func__, c->self, c->ctl_chan);

	 
	buffer_init(&m);
	buffer_put_int(&m, MUX_S_EXIT_MESSAGE);
	buffer_put_int(&m, c->self);
	buffer_put_int(&m, exitval);

	buffer_put_string(&mux_chan->output, buffer_ptr(&m), buffer_len(&m));
	buffer_free(&m);
}
