static void fwnet_transmit_packet_failed(struct fwnet_packet_task *ptask)
{
	struct fwnet_device *dev = ptask->dev;
	unsigned long flags;
	bool free;

	spin_lock_irqsave(&dev->lock, flags);

	 
	ptask->outstanding_pkts = 0;

	 
	free = ptask->enqueued;
	if (free)
		dec_queued_datagrams(dev);

	dev->netdev->stats.tx_dropped++;
	dev->netdev->stats.tx_errors++;

	spin_unlock_irqrestore(&dev->lock, flags);

	if (free)
		fwnet_free_ptask(ptask);
}
