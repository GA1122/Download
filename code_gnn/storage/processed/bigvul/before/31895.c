void tcp_shutdown(struct sock *sk, int how)
{
	 
	if (!(how & SEND_SHUTDOWN))
		return;

	 
	if ((1 << sk->sk_state) &
	    (TCPF_ESTABLISHED | TCPF_SYN_SENT |
	     TCPF_SYN_RECV | TCPF_CLOSE_WAIT)) {
		 
		if (tcp_close_state(sk))
			tcp_send_fin(sk);
	}
}
