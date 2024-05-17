static int sk_rcvbuf_lowwater(struct caifsock *cf_sk)
{
	 
	return cf_sk->sk.sk_rcvbuf / 4;
}
