mux_master_control_cleanup_cb(int cid, void *unused)
{
	Channel *sc, *c = channel_by_id(cid);

	debug3("%s: entering for channel %d", __func__, cid);
	if (c == NULL)
		fatal("%s: channel_by_id(%i) == NULL", __func__, cid);
	if (c->remote_id != -1) {
		if ((sc = channel_by_id(c->remote_id)) == NULL)
			fatal("%s: channel %d missing session channel %d",
			    __func__, c->self, c->remote_id);
		c->remote_id = -1;
		sc->ctl_chan = -1;
		if (sc->type != SSH_CHANNEL_OPEN &&
		    sc->type != SSH_CHANNEL_OPENING) {
			debug2("%s: channel %d: not open", __func__, sc->self);
			chan_mark_dead(sc);
		} else {
			if (sc->istate == CHAN_INPUT_OPEN)
				chan_read_failed(sc);
			if (sc->ostate == CHAN_OUTPUT_OPEN)
				chan_write_failed(sc);
		}
	}
	channel_cancel_cleanup(c->self);
}
