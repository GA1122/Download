static int packet_notifier(struct notifier_block *this, unsigned long msg, void *data)
{
	struct sock *sk;
	struct hlist_node *node;
	struct net_device *dev = data;
	struct net *net = dev_net(dev);

	rcu_read_lock();
	sk_for_each_rcu(sk, node, &net->packet.sklist) {
		struct packet_sock *po = pkt_sk(sk);

		switch (msg) {
		case NETDEV_UNREGISTER:
			if (po->mclist)
				packet_dev_mclist(dev, po->mclist, -1);
			 

		case NETDEV_DOWN:
			if (dev->ifindex == po->ifindex) {
				spin_lock(&po->bind_lock);
				if (po->running) {
					__dev_remove_pack(&po->prot_hook);
					__sock_put(sk);
					po->running = 0;
					sk->sk_err = ENETDOWN;
					if (!sock_flag(sk, SOCK_DEAD))
						sk->sk_error_report(sk);
				}
				if (msg == NETDEV_UNREGISTER) {
					po->ifindex = -1;
					po->prot_hook.dev = NULL;
				}
				spin_unlock(&po->bind_lock);
			}
			break;
		case NETDEV_UP:
			if (dev->ifindex == po->ifindex) {
				spin_lock(&po->bind_lock);
				if (po->num && !po->running) {
					dev_add_pack(&po->prot_hook);
					sock_hold(sk);
					po->running = 1;
				}
				spin_unlock(&po->bind_lock);
			}
			break;
		}
	}
	rcu_read_unlock();
	return NOTIFY_DONE;
}