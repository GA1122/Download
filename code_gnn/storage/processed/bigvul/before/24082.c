void stop_airo_card( struct net_device *dev, int freeres )
{
	struct airo_info *ai = dev->ml_priv;

	set_bit(FLAG_RADIO_DOWN, &ai->flags);
	disable_MAC(ai, 1);
	disable_interrupts(ai);
	takedown_proc_entry( dev, ai );
	if (test_bit(FLAG_REGISTERED, &ai->flags)) {
		unregister_netdev( dev );
		if (ai->wifidev) {
			unregister_netdev(ai->wifidev);
			free_netdev(ai->wifidev);
			ai->wifidev = NULL;
		}
		clear_bit(FLAG_REGISTERED, &ai->flags);
	}
	 
	if (test_bit(FLAG_MPI, &ai->flags) && !skb_queue_empty(&ai->txq)) {
		struct sk_buff *skb = NULL;
		for (;(skb = skb_dequeue(&ai->txq));)
			dev_kfree_skb(skb);
	}

	airo_networks_free (ai);

	kfree(ai->flash);
	kfree(ai->rssi);
	kfree(ai->APList);
	kfree(ai->SSID);
	if (freeres) {
		 
	        release_region( dev->base_addr, 64 );
		if (test_bit(FLAG_MPI, &ai->flags)) {
			if (ai->pci)
				mpi_unmap_card(ai->pci);
			if (ai->pcimem)
				iounmap(ai->pcimem);
			if (ai->pciaux)
				iounmap(ai->pciaux);
			pci_free_consistent(ai->pci, PCI_SHARED_LEN,
				ai->shared, ai->shared_dma);
		}
        }
	crypto_free_cipher(ai->tfm);
	del_airo_dev(ai);
	free_netdev( dev );
}