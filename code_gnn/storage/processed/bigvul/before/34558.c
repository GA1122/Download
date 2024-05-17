static void macvtap_del_queues(struct net_device *dev)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
	struct macvtap_queue *q, *qlist[MAX_MACVTAP_QUEUES];
	int i, j = 0;

	 
	spin_lock(&macvtap_lock);
	for (i = 0; i < MAX_MACVTAP_QUEUES && vlan->numvtaps; i++) {
		q = rcu_dereference_protected(vlan->taps[i],
					      lockdep_is_held(&macvtap_lock));
		if (q) {
			qlist[j++] = q;
			RCU_INIT_POINTER(vlan->taps[i], NULL);
			RCU_INIT_POINTER(q->vlan, NULL);
			vlan->numvtaps--;
		}
	}
	BUG_ON(vlan->numvtaps != 0);
	 
	vlan->numvtaps = MAX_MACVTAP_QUEUES;
	spin_unlock(&macvtap_lock);

	synchronize_rcu();

	for (--j; j >= 0; j--)
		sock_put(&qlist[j]->sk);
}