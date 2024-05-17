static ssize_t tun_do_read(struct tun_struct *tun,
			   struct kiocb *iocb, const struct iovec *iv,
			   ssize_t len, int noblock)
{
	DECLARE_WAITQUEUE(wait, current);
	struct sk_buff *skb;
	ssize_t ret = 0;

	tun_debug(KERN_INFO, tun, "tun_chr_read\n");

	if (unlikely(!noblock))
		add_wait_queue(&tun->wq.wait, &wait);
	while (len) {
		current->state = TASK_INTERRUPTIBLE;

		 
		if (!(skb=skb_dequeue(&tun->socket.sk->sk_receive_queue))) {
			if (noblock) {
				ret = -EAGAIN;
				break;
			}
			if (signal_pending(current)) {
				ret = -ERESTARTSYS;
				break;
			}
			if (tun->dev->reg_state != NETREG_REGISTERED) {
				ret = -EIO;
				break;
			}

			 
			schedule();
			continue;
		}
		netif_wake_queue(tun->dev);

		ret = tun_put_user(tun, skb, iv, len);
		kfree_skb(skb);
		break;
	}

	current->state = TASK_RUNNING;
	if (unlikely(!noblock))
		remove_wait_queue(&tun->wq.wait, &wait);

	return ret;
}