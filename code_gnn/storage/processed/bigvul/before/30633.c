static int irda_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
	struct sock *sk = sock->sk;
	struct sockaddr_irda *addr = (struct sockaddr_irda *) uaddr;
	struct irda_sock *self = irda_sk(sk);
	int err;

	IRDA_DEBUG(2, "%s(%p)\n", __func__, self);

	if (addr_len != sizeof(struct sockaddr_irda))
		return -EINVAL;

	lock_sock(sk);
#ifdef CONFIG_IRDA_ULTRA
	 
	if ((sk->sk_type == SOCK_DGRAM) &&
	    (sk->sk_protocol == IRDAPROTO_ULTRA)) {
		self->pid = addr->sir_lsap_sel;
		err = -EOPNOTSUPP;
		if (self->pid & 0x80) {
			IRDA_DEBUG(0, "%s(), extension in PID not supp!\n", __func__);
			goto out;
		}
		err = irda_open_lsap(self, self->pid);
		if (err < 0)
			goto out;

		 
		sock->state = SS_CONNECTED;
		sk->sk_state   = TCP_ESTABLISHED;
		err = 0;

		goto out;
	}
#endif  

	self->ias_obj = irias_new_object(addr->sir_name, jiffies);
	err = -ENOMEM;
	if (self->ias_obj == NULL)
		goto out;

	err = irda_open_tsap(self, addr->sir_lsap_sel, addr->sir_name);
	if (err < 0) {
		irias_delete_object(self->ias_obj);
		self->ias_obj = NULL;
		goto out;
	}

	 
	irias_add_integer_attrib(self->ias_obj, "IrDA:TinyTP:LsapSel",
				 self->stsap_sel, IAS_KERNEL_ATTR);
	irias_insert_object(self->ias_obj);

	err = 0;
out:
	release_sock(sk);
	return err;
}