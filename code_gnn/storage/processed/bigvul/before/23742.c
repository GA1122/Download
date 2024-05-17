static void bond_resend_igmp_join_requests(struct bonding *bond)
{
	struct net_device *vlan_dev;
	struct vlan_entry *vlan;

	read_lock(&bond->lock);

	 
	__bond_resend_igmp_join_requests(bond->dev);

	 
	list_for_each_entry(vlan, &bond->vlan_list, vlan_list) {
		rcu_read_lock();
		vlan_dev = __vlan_find_dev_deep(bond->dev,
						vlan->vlan_id);
		rcu_read_unlock();
		if (vlan_dev)
			__bond_resend_igmp_join_requests(vlan_dev);
	}

	if (--bond->igmp_retrans > 0)
		queue_delayed_work(bond->wq, &bond->mcast_work, HZ/5);

	read_unlock(&bond->lock);
}