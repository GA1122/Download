process_mux_alive_check(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	debug2("%s: channel %d: alive check", __func__, c->self);

	 
	buffer_put_int(r, MUX_S_ALIVE);
	buffer_put_int(r, rid);
	buffer_put_int(r, (u_int)getpid());

	return 0;
}
