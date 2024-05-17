mux_master_session_cleanup_cb(int cid, void *unused)
{
	Channel *cc, *c = channel_by_id(cid);

	debug3("%s: entering for channel %d", __func__, cid);
	if (c == NULL)
		fatal("%s: channel_by_id(%i) == NULL", __func__, cid);
	if (c->ctl_chan != -1) {
		if ((cc = channel_by_id(c->ctl_chan)) == NULL)
			fatal("%s: channel %d missing control channel %d",
			    __func__, c->self, c->ctl_chan);
		c->ctl_chan = -1;
		cc->remote_id = -1;
		chan_rcvd_oclose(cc);
	}
	channel_cancel_cleanup(c->self);
}
