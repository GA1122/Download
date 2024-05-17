static int rt_cache_seq_show(struct seq_file *seq, void *v)
{
	if (v == SEQ_START_TOKEN)
		seq_printf(seq, "%-127s\n",
			   "Iface\tDestination\tGateway \tFlags\t\tRefCnt\tUse\t"
			   "Metric\tSource\t\tMTU\tWindow\tIRTT\tTOS\tHHRef\t"
			   "HHUptod\tSpecDst");
	else {
		struct rtable *r = v;
		struct neighbour *n;
		int len;

		n = dst_get_neighbour(&r->dst);
		seq_printf(seq, "%s\t%08X\t%08X\t%8X\t%d\t%u\t%d\t"
			      "%08X\t%d\t%u\t%u\t%02X\t%d\t%1d\t%08X%n",
			r->dst.dev ? r->dst.dev->name : "*",
			(__force u32)r->rt_dst,
			(__force u32)r->rt_gateway,
			r->rt_flags, atomic_read(&r->dst.__refcnt),
			r->dst.__use, 0, (__force u32)r->rt_src,
			dst_metric_advmss(&r->dst) + 40,
			dst_metric(&r->dst, RTAX_WINDOW),
			(int)((dst_metric(&r->dst, RTAX_RTT) >> 3) +
			      dst_metric(&r->dst, RTAX_RTTVAR)),
			r->rt_key_tos,
			-1,
			(n && (n->nud_state & NUD_CONNECTED)) ? 1 : 0,
			r->rt_spec_dst, &len);

		seq_printf(seq, "%*s\n", 127 - len, "");
	}
	return 0;
}
