static int dn_auto_bind(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct dn_scp *scp = DN_SK(sk);
	int rv;

	sock_reset_flag(sk, SOCK_ZAPPED);

	scp->addr.sdn_flags  = 0;
	scp->addr.sdn_objnum = 0;

	 
	if ((scp->accessdata.acc_accl != 0) &&
		(scp->accessdata.acc_accl <= 12)) {

		scp->addr.sdn_objnamel = cpu_to_le16(scp->accessdata.acc_accl);
		memcpy(scp->addr.sdn_objname, scp->accessdata.acc_acc, le16_to_cpu(scp->addr.sdn_objnamel));

		scp->accessdata.acc_accl = 0;
		memset(scp->accessdata.acc_acc, 0, 40);
	}
	 

	scp->addr.sdn_add.a_len = cpu_to_le16(2);
	rv = dn_dev_bind_default((__le16 *)scp->addr.sdn_add.a_addr);
	if (rv == 0) {
		rv = dn_hash_sock(sk);
		if (rv)
			sock_set_flag(sk, SOCK_ZAPPED);
	}

	return rv;
}