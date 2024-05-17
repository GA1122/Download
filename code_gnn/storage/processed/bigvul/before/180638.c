  static int rfcomm_sock_bind(struct socket *sock, struct sockaddr *addr, int addr_len)
  {
	struct sockaddr_rc *sa = (struct sockaddr_rc *) addr;
// 	struct sockaddr_rc sa;
  	struct sock *sk = sock->sk;
	int chan = sa->rc_channel;
	int err = 0;
	BT_DBG("sk %p %pMR", sk, &sa->rc_bdaddr);
// 	int len, err = 0;
  
  	if (!addr || addr->sa_family != AF_BLUETOOTH)
  		return -EINVAL;
  
// 	memset(&sa, 0, sizeof(sa));
// 	len = min_t(unsigned int, sizeof(sa), addr_len);
// 	memcpy(&sa, addr, len);
// 
// 	BT_DBG("sk %p %pMR", sk, &sa.rc_bdaddr);
// 
  	lock_sock(sk);
  
  	if (sk->sk_state != BT_OPEN) {
 		err = -EBADFD;
 		goto done;
 	}
 
 	if (sk->sk_type != SOCK_STREAM) {
 		err = -EINVAL;
 		goto done;
 	}
  
  	write_lock(&rfcomm_sk_list.lock);
  
	if (chan && __rfcomm_get_listen_sock_by_addr(chan, &sa->rc_bdaddr)) {
// 	if (sa.rc_channel &&
// 	    __rfcomm_get_listen_sock_by_addr(sa.rc_channel, &sa.rc_bdaddr)) {
  		err = -EADDRINUSE;
  	} else {
  		 
		bacpy(&rfcomm_pi(sk)->src, &sa->rc_bdaddr);
		rfcomm_pi(sk)->channel = chan;
// 		bacpy(&rfcomm_pi(sk)->src, &sa.rc_bdaddr);
// 		rfcomm_pi(sk)->channel = sa.rc_channel;
  		sk->sk_state = BT_BOUND;
  	}
  
 	write_unlock(&rfcomm_sk_list.lock);
 
 done:
 	release_sock(sk);
 	return err;
 }