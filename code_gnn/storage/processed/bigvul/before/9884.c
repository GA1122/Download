void stream_int_retnclose(struct stream_interface *si, const struct chunk *msg)
{
	channel_auto_read(si->ib);
	channel_abort(si->ib);
	channel_auto_close(si->ib);
	channel_erase(si->ib);

	bi_erase(si->ob);
	if (likely(msg && msg->len))
		bo_inject(si->ob, msg->str, msg->len);

	si->ob->wex = tick_add_ifset(now_ms, si->ob->wto);
	channel_auto_read(si->ob);
	channel_auto_close(si->ob);
	channel_shutr_now(si->ob);
}
