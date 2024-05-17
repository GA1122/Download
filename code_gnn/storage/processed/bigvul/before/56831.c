static void usb_set_lpm_mel(struct usb_device *udev,
		struct usb3_lpm_parameters *udev_lpm_params,
		unsigned int udev_exit_latency,
		struct usb_hub *hub,
		struct usb3_lpm_parameters *hub_lpm_params,
		unsigned int hub_exit_latency)
{
	unsigned int total_mel;
	unsigned int device_mel;
	unsigned int hub_mel;

	 
	total_mel = hub_lpm_params->mel +
		(hub->descriptor->u.ss.bHubHdrDecLat * 100);

	 
	device_mel = udev_exit_latency * 1000;
	hub_mel = hub_exit_latency * 1000;
	if (device_mel > hub_mel)
		total_mel += device_mel;
	else
		total_mel += hub_mel;

	udev_lpm_params->mel = total_mel;
}
