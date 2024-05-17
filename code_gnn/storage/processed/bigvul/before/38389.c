static void cm_get_ack_delay(struct cm_device *cm_dev)
{
	struct ib_device_attr attr;

	if (ib_query_device(cm_dev->ib_device, &attr))
		cm_dev->ack_delay = 0;  
	else
		cm_dev->ack_delay = attr.local_ca_ack_delay;
}
