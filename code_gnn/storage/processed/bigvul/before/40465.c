static struct sock *ncp_connection_hack(struct ipx_interface *intrfc,
					struct ipxhdr *ipx)
{
	 
	struct sock *sk = NULL;
	int connection = 0;
	u8 *ncphdr = (u8 *)(ipx + 1);

	if (*ncphdr == 0x22 && *(ncphdr + 1) == 0x22)  
		connection = (((int) *(ncphdr + 5)) << 8) | (int) *(ncphdr + 3);
	else if (*ncphdr == 0x77 && *(ncphdr + 1) == 0x77)  
		connection = (((int) *(ncphdr + 9)) << 8) | (int) *(ncphdr + 8);

	if (connection) {
		 
		spin_lock_bh(&intrfc->if_sklist_lock);
		sk_for_each(sk, &intrfc->if_sklist)
			if (ipx_sk(sk)->ipx_ncp_conn == connection) {
				sock_hold(sk);
				goto found;
			}
		sk = NULL;
	found:
		spin_unlock_bh(&intrfc->if_sklist_lock);
	}
	return sk;
}
