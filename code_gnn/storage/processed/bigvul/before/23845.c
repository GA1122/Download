static int tun_chr_close(struct inode *inode, struct file *file)
{
	struct tun_file *tfile = file->private_data;
	struct tun_struct *tun;

	tun = __tun_get(tfile);
	if (tun) {
		struct net_device *dev = tun->dev;

		tun_debug(KERN_INFO, tun, "tun_chr_close\n");

		__tun_detach(tun);

		 
		if (!(tun->flags & TUN_PERSIST)) {
			rtnl_lock();
			if (dev->reg_state == NETREG_REGISTERED)
				unregister_netdevice(dev);
			rtnl_unlock();
		}
	}

	tun = tfile->tun;
	if (tun)
		sock_put(tun->socket.sk);

	put_net(tfile->net);
	kfree(tfile);

	return 0;
}