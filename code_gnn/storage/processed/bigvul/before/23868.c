static void tun_net_uninit(struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);
	struct tun_file *tfile = tun->tfile;

	 
	if (tfile) {
		wake_up_all(&tun->wq.wait);
		if (atomic_dec_and_test(&tfile->count))
			__tun_detach(tun);
	}
}