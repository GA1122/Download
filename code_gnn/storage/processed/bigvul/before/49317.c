static int fwnet_broadcast_start(struct fwnet_device *dev)
{
	struct fw_iso_context *context;
	int retval;
	unsigned num_packets;
	unsigned max_receive;
	struct fw_iso_packet packet;
	unsigned long offset;
	void **ptrptr;
	unsigned u;

	if (dev->broadcast_state != FWNET_BROADCAST_ERROR)
		return 0;

	max_receive = 1U << (dev->card->max_receive + 1);
	num_packets = (FWNET_ISO_PAGE_COUNT * PAGE_SIZE) / max_receive;

	ptrptr = kmalloc(sizeof(void *) * num_packets, GFP_KERNEL);
	if (!ptrptr) {
		retval = -ENOMEM;
		goto failed;
	}
	dev->broadcast_rcv_buffer_ptrs = ptrptr;

	context = fw_iso_context_create(dev->card, FW_ISO_CONTEXT_RECEIVE,
					IEEE1394_BROADCAST_CHANNEL,
					dev->card->link_speed, 8,
					fwnet_receive_broadcast, dev);
	if (IS_ERR(context)) {
		retval = PTR_ERR(context);
		goto failed;
	}

	retval = fw_iso_buffer_init(&dev->broadcast_rcv_buffer, dev->card,
				    FWNET_ISO_PAGE_COUNT, DMA_FROM_DEVICE);
	if (retval < 0)
		goto failed;

	dev->broadcast_state = FWNET_BROADCAST_STOPPED;

	for (u = 0; u < FWNET_ISO_PAGE_COUNT; u++) {
		void *ptr;
		unsigned v;

		ptr = kmap(dev->broadcast_rcv_buffer.pages[u]);
		for (v = 0; v < num_packets / FWNET_ISO_PAGE_COUNT; v++)
			*ptrptr++ = (void *) ((char *)ptr + v * max_receive);
	}
	dev->broadcast_rcv_context = context;

	packet.payload_length = max_receive;
	packet.interrupt = 1;
	packet.skip = 0;
	packet.tag = 3;
	packet.sy = 0;
	packet.header_length = IEEE1394_GASP_HDR_SIZE;
	offset = 0;

	for (u = 0; u < num_packets; u++) {
		retval = fw_iso_context_queue(context, &packet,
				&dev->broadcast_rcv_buffer, offset);
		if (retval < 0)
			goto failed;

		offset += max_receive;
	}
	dev->num_broadcast_rcv_ptrs = num_packets;
	dev->rcv_buffer_size = max_receive;
	dev->broadcast_rcv_next_ptr = 0U;
	retval = fw_iso_context_start(context, -1, 0,
			FW_ISO_CONTEXT_MATCH_ALL_TAGS);  
	if (retval < 0)
		goto failed;

	 
	dev->broadcast_xmt_max_payload = IEEE1394_MAX_PAYLOAD_S100
			- IEEE1394_GASP_HDR_SIZE - RFC2374_UNFRAG_HDR_SIZE;
	dev->broadcast_state = FWNET_BROADCAST_RUNNING;

	return 0;

 failed:
	__fwnet_broadcast_stop(dev);
	return retval;
}