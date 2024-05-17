static int init_device(struct libusb_device* dev, struct libusb_device* parent_dev,
					   uint8_t port_number, char* device_id, DWORD devinst)
{
	HANDLE handle;
	DWORD size;
	USB_NODE_CONNECTION_INFORMATION_EX conn_info;
	struct windows_device_priv *priv, *parent_priv;
	struct libusb_context *ctx = DEVICE_CTX(dev);
	struct libusb_device* tmp_dev;
	unsigned i;

	if ((dev == NULL) || (parent_dev == NULL)) {
		return LIBUSB_ERROR_NOT_FOUND;
	}
	priv = _device_priv(dev);
	parent_priv = _device_priv(parent_dev);
	if (parent_priv->apib->id != USB_API_HUB) {
		usbi_warn(ctx, "parent for device '%s' is not a hub", device_id);
		return LIBUSB_ERROR_NOT_FOUND;
	}

	if (parent_dev->bus_number == 0) {
		for (i=2; ; i++) {
			tmp_dev = usbi_get_device_by_session_id(ctx, get_ancestor_session_id(devinst, i));
			if (tmp_dev == NULL) break;
			if (tmp_dev->bus_number != 0) {
				usbi_dbg("got bus number from ancestor #%d", i);
				parent_dev->bus_number = tmp_dev->bus_number;
				break;
			}
		}
	}
	if (parent_dev->bus_number == 0) {
		usbi_err(ctx, "program assertion failed: unable to find ancestor bus number for '%s'", device_id);
		return LIBUSB_ERROR_NOT_FOUND;
	}
	dev->bus_number = parent_dev->bus_number;
	priv->port = port_number;
	dev->port_number = port_number;
	priv->depth = parent_priv->depth + 1;
	priv->parent_dev = parent_dev;
	dev->parent_dev = libusb_ref_device(parent_dev);

	if (dev->device_address != 0) {
		return LIBUSB_SUCCESS;
	}
	memset(&conn_info, 0, sizeof(conn_info));
	if (priv->depth != 0) {	 
		handle = CreateFileA(parent_priv->path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED, NULL);
		if (handle == INVALID_HANDLE_VALUE) {
			usbi_warn(ctx, "could not open hub %s: %s", parent_priv->path, windows_error_str(0));
			return LIBUSB_ERROR_ACCESS;
		}
		size = sizeof(conn_info);
		conn_info.ConnectionIndex = (ULONG)port_number;
		if (!DeviceIoControl(handle, IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX, &conn_info, size,
			&conn_info, size, &size, NULL)) {
			usbi_warn(ctx, "could not get node connection information for device '%s': %s",
				device_id, windows_error_str(0));
			safe_closehandle(handle);
			return LIBUSB_ERROR_NO_DEVICE;
		}
		if (conn_info.ConnectionStatus == NoDeviceConnected) {
			usbi_err(ctx, "device '%s' is no longer connected!", device_id);
			safe_closehandle(handle);
			return LIBUSB_ERROR_NO_DEVICE;
		}
		memcpy(&priv->dev_descriptor, &(conn_info.DeviceDescriptor), sizeof(USB_DEVICE_DESCRIPTOR));
		dev->num_configurations = priv->dev_descriptor.bNumConfigurations;
		priv->active_config = conn_info.CurrentConfigurationValue;
		usbi_dbg("found %d configurations (active conf: %d)", dev->num_configurations, priv->active_config);
		if (cache_config_descriptors(dev, handle, device_id) != LIBUSB_SUCCESS) {
			dev->num_configurations = 0;
			priv->dev_descriptor.bNumConfigurations = 0;
		}
		safe_closehandle(handle);

		if (conn_info.DeviceAddress > UINT8_MAX) {
			usbi_err(ctx, "program assertion failed: device address overflow");
		}
		dev->device_address = (uint8_t)conn_info.DeviceAddress + 1;
		if (dev->device_address == 1) {
			usbi_err(ctx, "program assertion failed: device address collision with root hub");
		}
		switch (conn_info.Speed) {
		case 0: dev->speed = LIBUSB_SPEED_LOW; break;
		case 1: dev->speed = LIBUSB_SPEED_FULL; break;
		case 2: dev->speed = LIBUSB_SPEED_HIGH; break;
		case 3: dev->speed = LIBUSB_SPEED_SUPER; break;
		default:
			usbi_warn(ctx, "Got unknown device speed %d", conn_info.Speed);
			break;
		}
	} else {
		dev->device_address = 1;	 
		force_hcd_device_descriptor(dev);
	}

	usbi_sanitize_device(dev);

	usbi_dbg("(bus: %d, addr: %d, depth: %d, port: %d): '%s'",
		dev->bus_number, dev->device_address, priv->depth, priv->port, device_id);

	return LIBUSB_SUCCESS;
}
