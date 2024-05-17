static void airo_handle_tx(struct airo_info *ai, u16 status)
{
	int i, len = 0, index = -1;
	u16 fid;

	if (test_bit(FLAG_MPI, &ai->flags)) {
		unsigned long flags;

		if (status & EV_TXEXC)
			get_tx_error(ai, -1);

		spin_lock_irqsave(&ai->aux_lock, flags);
		if (!skb_queue_empty(&ai->txq)) {
			spin_unlock_irqrestore(&ai->aux_lock,flags);
			mpi_send_packet(ai->dev);
		} else {
			clear_bit(FLAG_PENDING_XMIT, &ai->flags);
			spin_unlock_irqrestore(&ai->aux_lock,flags);
			netif_wake_queue(ai->dev);
		}
		OUT4500(ai, EVACK, status & (EV_TX | EV_TXCPY | EV_TXEXC));
		return;
	}

	fid = IN4500(ai, TXCOMPLFID);

	for(i = 0; i < MAX_FIDS; i++) {
		if ((ai->fids[i] & 0xffff) == fid) {
			len = ai->fids[i] >> 16;
			index = i;
		}
	}

	if (index != -1) {
		if (status & EV_TXEXC)
			get_tx_error(ai, index);

		OUT4500(ai, EVACK, status & (EV_TX | EV_TXEXC));

		 
		ai->fids[index] &= 0xffff;
		if (index < MAX_FIDS / 2) {
			if (!test_bit(FLAG_PENDING_XMIT, &ai->flags))
				netif_wake_queue(ai->dev);
		} else {
			if (!test_bit(FLAG_PENDING_XMIT11, &ai->flags))
				netif_wake_queue(ai->wifidev);
		}
	} else {
		OUT4500(ai, EVACK, status & (EV_TX | EV_TXCPY | EV_TXEXC));
		airo_print_err(ai->dev->name, "Unallocated FID was used to xmit");
	}
}