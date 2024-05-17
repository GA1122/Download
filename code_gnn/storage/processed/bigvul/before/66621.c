static void unplug_port(struct port *port)
{
	spin_lock_irq(&port->portdev->ports_lock);
	list_del(&port->list);
	spin_unlock_irq(&port->portdev->ports_lock);

	spin_lock_irq(&port->inbuf_lock);
	if (port->guest_connected) {
		 
		send_sigio_to_port(port);

		 
		port->guest_connected = false;
		port->host_connected = false;

		wake_up_interruptible(&port->waitqueue);
	}
	spin_unlock_irq(&port->inbuf_lock);

	if (is_console_port(port)) {
		spin_lock_irq(&pdrvdata_lock);
		list_del(&port->cons.list);
		spin_unlock_irq(&pdrvdata_lock);
		hvc_remove(port->cons.hvc);
	}

	remove_port_data(port);

	 
	port->portdev = NULL;

	sysfs_remove_group(&port->dev->kobj, &port_attribute_group);
	device_destroy(pdrvdata.class, port->dev->devt);
	cdev_del(port->cdev);

	debugfs_remove(port->debugfs_file);
	kfree(port->name);

	 
	kref_put(&port->kref, remove_port);
}
