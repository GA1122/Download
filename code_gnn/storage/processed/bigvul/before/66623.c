static int virtcons_freeze(struct virtio_device *vdev)
{
	struct ports_device *portdev;
	struct port *port;

	portdev = vdev->priv;

	vdev->config->reset(vdev);

	virtqueue_disable_cb(portdev->c_ivq);
	cancel_work_sync(&portdev->control_work);
	cancel_work_sync(&portdev->config_work);
	 
	virtqueue_disable_cb(portdev->c_ivq);
	remove_controlq_data(portdev);

	list_for_each_entry(port, &portdev->ports, list) {
		virtqueue_disable_cb(port->in_vq);
		virtqueue_disable_cb(port->out_vq);
		 
		port->host_connected = false;
		remove_port_data(port);
	}
	remove_vqs(portdev);

	return 0;
}
