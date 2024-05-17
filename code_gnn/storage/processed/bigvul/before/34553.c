static int get_slot(struct macvlan_dev *vlan, struct macvtap_queue *q)
{
	int i;

	for (i = 0; i < MAX_MACVTAP_QUEUES; i++) {
		if (rcu_dereference(vlan->taps[i]) == q)
			return i;
	}

	 
	BUG_ON(1);
}