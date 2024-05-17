static bool fwnet_pd_update(struct fwnet_peer *peer,
		struct fwnet_partial_datagram *pd, void *frag_buf,
		unsigned frag_off, unsigned frag_len)
{
	if (fwnet_frag_new(pd, frag_off, frag_len) == NULL)
		return false;

	memcpy(pd->pbuf + frag_off, frag_buf, frag_len);

	 
	list_move_tail(&pd->pd_link, &peer->pd_list);

	return true;
}