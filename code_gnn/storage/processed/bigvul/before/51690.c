static void remember_received_packet(struct state *st, struct msg_digest *md)
{
	if (md->encrypted) {
		 
		if (md->raw_packet.ptr != NULL) {
			pfreeany(st->st_rpacket.ptr);
			st->st_rpacket = md->raw_packet;
			md->raw_packet.ptr = NULL;
		}
	} else {
		 
		pfreeany(st->st_rpacket.ptr);
		clonetochunk(st->st_rpacket,
			     md->packet_pbs.start,
			     pbs_room(&md->packet_pbs), "raw packet");
	}
}
