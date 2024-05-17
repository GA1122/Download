static void l2cap_sock_init(struct sock *sk, struct sock *parent)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p", sk);

	if (parent) {
		sk->sk_type = parent->sk_type;
		bt_sk(sk)->defer_setup = bt_sk(parent)->defer_setup;

		pi->imtu = l2cap_pi(parent)->imtu;
		pi->omtu = l2cap_pi(parent)->omtu;
		pi->mode = l2cap_pi(parent)->mode;
		pi->fcs  = l2cap_pi(parent)->fcs;
		pi->sec_level = l2cap_pi(parent)->sec_level;
		pi->role_switch = l2cap_pi(parent)->role_switch;
		pi->force_reliable = l2cap_pi(parent)->force_reliable;
	} else {
		pi->imtu = L2CAP_DEFAULT_MTU;
		pi->omtu = 0;
		pi->mode = L2CAP_MODE_BASIC;
		pi->fcs  = L2CAP_FCS_CRC16;
		pi->sec_level = BT_SECURITY_LOW;
		pi->role_switch = 0;
		pi->force_reliable = 0;
	}

	 
	pi->conf_len = 0;
	pi->flush_to = L2CAP_DEFAULT_FLUSH_TO;
}