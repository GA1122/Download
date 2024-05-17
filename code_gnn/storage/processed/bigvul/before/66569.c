static void write_bulk_callback(struct urb *urb)
{
	pegasus_t *pegasus = urb->context;
	struct net_device *net;
	int status = urb->status;

	if (!pegasus)
		return;

	net = pegasus->net;

	if (!netif_device_present(net) || !netif_running(net))
		return;

	switch (status) {
	case -EPIPE:
		 
		netif_stop_queue(net);
		netif_warn(pegasus, tx_err, net, "no tx stall recovery\n");
		return;
	case -ENOENT:
	case -ECONNRESET:
	case -ESHUTDOWN:
		netif_dbg(pegasus, ifdown, net, "tx unlink, %d\n", status);
		return;
	default:
		netif_info(pegasus, tx_err, net, "TX status %d\n", status);
		 
	case 0:
		break;
	}

	netif_trans_update(net);  
	netif_wake_queue(net);
}