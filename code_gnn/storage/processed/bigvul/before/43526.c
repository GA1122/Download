static void sctp_destruct_sock(struct sock *sk)
{
	struct sctp_sock *sp = sctp_sk(sk);

	 
	crypto_free_hash(sp->hmac);

	inet_sock_destruct(sk);
}
