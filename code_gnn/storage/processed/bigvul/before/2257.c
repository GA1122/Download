channel_register_open_confirm(int id, channel_open_fn *fn, void *ctx)
{
	Channel *c = channel_lookup(id);

	if (c == NULL) {
		logit("channel_register_open_confirm: %d: bad id", id);
		return;
	}
	c->open_confirm = fn;
	c->open_confirm_ctx = ctx;
}
