static void __tun_detach(struct tun_struct *tun)
{
	 
	netif_tx_lock_bh(tun->dev);
	netif_carrier_off(tun->dev);
	tun->tfile = NULL;
	tun->socket.file = NULL;
	netif_tx_unlock_bh(tun->dev);

	 
	skb_queue_purge(&tun->socket.sk->sk_receive_queue);

	 
	dev_put(tun->dev);
}
