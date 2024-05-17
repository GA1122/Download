static void get_openreq4(struct sock *sk, struct request_sock *req,
			 struct seq_file *f, int i, int uid, int *len)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
	int ttd = req->expires - jiffies;

	seq_printf(f, "%4d: %08X:%04X %08X:%04X"
		" %02X %08X:%08X %02X:%08lX %08X %5d %8d %u %d %pK%n",
		i,
		ireq->loc_addr,
		ntohs(inet_sk(sk)->inet_sport),
		ireq->rmt_addr,
		ntohs(ireq->rmt_port),
		TCP_SYN_RECV,
		0, 0,  
		1,     
		jiffies_to_clock_t(ttd),
		req->retrans,
		uid,
		0,   
		0,  
		atomic_read(&sk->sk_refcnt),
		req,
		len);
}