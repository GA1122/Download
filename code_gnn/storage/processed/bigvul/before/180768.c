  static int unix_attach_fds(struct scm_cookie *scm, struct sk_buff *skb)
 {
 	int i;
  	unsigned char max_level = 0;
  	int unix_sock_count = 0;
  
// 	if (too_many_unix_fds(current))
// 		return -ETOOMANYREFS;
// 
  	for (i = scm->fp->count - 1; i >= 0; i--) {
  		struct sock *sk = unix_get_socket(scm->fp->fp[i]);
  
 		if (sk) {
 			unix_sock_count++;
 			max_level = max(max_level,
 					unix_sk(sk)->recursion_level);
 		}
 	}
 	if (unlikely(max_level > MAX_RECURSION_LEVEL))
 		return -ETOOMANYREFS;
 
 	 
 	UNIXCB(skb).fp = scm_fp_dup(scm->fp);
  	if (!UNIXCB(skb).fp)
  		return -ENOMEM;
  
	if (unix_sock_count) {
		for (i = scm->fp->count - 1; i >= 0; i--)
			unix_inflight(scm->fp->fp[i]);
	}
// 	for (i = scm->fp->count - 1; i >= 0; i--)
// 		unix_inflight(scm->fp->fp[i]);
  	return max_level;
  }