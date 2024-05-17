static void log_static_leases(struct static_lease **st_lease_pp)
{
	struct static_lease *cur;

	if (dhcp_verbose < 2)
		return;

	cur = *st_lease_pp;
	while (cur) {
		bb_error_msg("static lease: mac:%02x:%02x:%02x:%02x:%02x:%02x nip:%x",
			cur->mac[0], cur->mac[1], cur->mac[2],
			cur->mac[3], cur->mac[4], cur->mac[5],
			cur->nip
		);
		cur = cur->next;
	}
}
