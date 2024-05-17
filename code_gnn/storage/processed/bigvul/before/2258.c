channel_register_status_confirm(int id, channel_confirm_cb *cb,
    channel_confirm_abandon_cb *abandon_cb, void *ctx)
{
	struct channel_confirm *cc;
	Channel *c;

	if ((c = channel_lookup(id)) == NULL)
		fatal("channel_register_expect: %d: bad id", id);

	cc = xcalloc(1, sizeof(*cc));
	cc->cb = cb;
	cc->abandon_cb = abandon_cb;
	cc->ctx = ctx;
	TAILQ_INSERT_TAIL(&c->status_confirms, cc, entry);
}
