static void fanout_release_data(struct packet_fanout *f)
{
	switch (f->type) {
	case PACKET_FANOUT_CBPF:
	case PACKET_FANOUT_EBPF:
		__fanout_set_data_bpf(f, NULL);
	};
}