static SP_DEVICE_INTERFACE_DETAIL_DATA_A *get_interface_details_filter(struct libusb_context *ctx,
	HDEVINFO *dev_info, SP_DEVINFO_DATA *dev_info_data, const GUID* guid, unsigned _index, char* filter_path){
	SP_DEVICE_INTERFACE_DATA dev_interface_data;
	SP_DEVICE_INTERFACE_DETAIL_DATA_A *dev_interface_details = NULL;
	DWORD size;
	if (_index <= 0) {
		*dev_info = pSetupDiGetClassDevsA(guid, NULL, NULL, DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
	}
	if (dev_info_data != NULL) {
		dev_info_data->cbSize = sizeof(SP_DEVINFO_DATA);
		if (!pSetupDiEnumDeviceInfo(*dev_info, _index, dev_info_data)) {
			if (GetLastError() != ERROR_NO_MORE_ITEMS) {
				usbi_err(ctx, "Could not obtain device info data for index %u: %s",
					_index, windows_error_str(0));
			}
			pSetupDiDestroyDeviceInfoList(*dev_info);
			*dev_info = INVALID_HANDLE_VALUE;
			return NULL;
		}
	}
	dev_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	if (!pSetupDiEnumDeviceInterfaces(*dev_info, NULL, guid, _index, &dev_interface_data)) {
		if (GetLastError() != ERROR_NO_MORE_ITEMS) {
			usbi_err(ctx, "Could not obtain interface data for index %u: %s",
				_index, windows_error_str(0));
		}
		pSetupDiDestroyDeviceInfoList(*dev_info);
		*dev_info = INVALID_HANDLE_VALUE;
		return NULL;
	}
	if (!pSetupDiGetDeviceInterfaceDetailA(*dev_info, &dev_interface_data, NULL, 0, &size, NULL)) {
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			usbi_err(ctx, "could not access interface data (dummy) for index %u: %s",
				_index, windows_error_str(0));
			goto err_exit;
		}
	} else {
		usbi_err(ctx, "program assertion failed - http://msdn.microsoft.com/en-us/library/ms792901.aspx is wrong.");
		goto err_exit;
	}
	if ((dev_interface_details = malloc(size)) == NULL) {
		usbi_err(ctx, "could not allocate interface data for index %u.", _index);
		goto err_exit;
	}
	dev_interface_details->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
	if (!pSetupDiGetDeviceInterfaceDetailA(*dev_info, &dev_interface_data,
		dev_interface_details, size, &size, NULL)) {
		usbi_err(ctx, "could not access interface data (actual) for index %u: %s",
			_index, windows_error_str(0));
	}
	if (dev_interface_details) {
		HKEY hkey_device_interface=pSetupDiOpenDeviceInterfaceRegKey(*dev_info,&dev_interface_data,0,KEY_READ);
		if (hkey_device_interface != INVALID_HANDLE_VALUE) {
			DWORD libusb0_symboliclink_index=0;
			DWORD value_length=sizeof(DWORD);
			DWORD value_type=0;
			LONG status;
			status = pRegQueryValueExW(hkey_device_interface, L"LUsb0", NULL, &value_type,
				(LPBYTE) &libusb0_symboliclink_index, &value_length);
			if (status == ERROR_SUCCESS) {
				if (libusb0_symboliclink_index < 256) {
					safe_sprintf(filter_path, sizeof("\\\\.\\libusb0-0000"), "\\\\.\\libusb0-%04d", libusb0_symboliclink_index);
					usbi_dbg("assigned libusb0 symbolic link %s", filter_path);
				} else {
				}
			}
			pRegCloseKey(hkey_device_interface);
		}
	}
	return dev_interface_details;
err_exit:
	pSetupDiDestroyDeviceInfoList(*dev_info);
	*dev_info = INVALID_HANDLE_VALUE;
	return NULL;}
