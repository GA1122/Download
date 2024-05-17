static void isdn_net_tx_timeout(struct net_device * ndev)
{
	isdn_net_local *lp = netdev_priv(ndev);

	printk(KERN_WARNING "isdn_tx_timeout dev %s dialstate %d\n", ndev->name, lp->dialstate);
	if (!lp->dialstate){
		lp->stats.tx_errors++;
                 
	}
	ndev->trans_start = jiffies;
	netif_wake_queue(ndev);
}
