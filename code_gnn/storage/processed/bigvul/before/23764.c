static void bond_uninit(struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct vlan_entry *vlan, *tmp;

	bond_netpoll_cleanup(bond_dev);

	 
	bond_release_all(bond_dev);

	list_del(&bond->bond_list);

	bond_work_cancel_all(bond);

	bond_remove_proc_entry(bond);

	bond_debug_unregister(bond);

	__hw_addr_flush(&bond->mc_list);

	list_for_each_entry_safe(vlan, tmp, &bond->vlan_list, vlan_list) {
		list_del(&vlan->vlan_list);
		kfree(vlan);
	}
}