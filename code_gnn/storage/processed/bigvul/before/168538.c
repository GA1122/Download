static bool get_devinfo_data(struct libusb_context *ctx,
	HDEVINFO *dev_info, SP_DEVINFO_DATA *dev_info_data, const char* usb_class, unsigned _index)
{
	if (_index <= 0) {
		*dev_info = pSetupDiGetClassDevsA(NULL, usb_class, NULL, DIGCF_PRESENT|DIGCF_ALLCLASSES);
		if (*dev_info == INVALID_HANDLE_VALUE) {
			return false;
		}
	}

	dev_info_data->cbSize = sizeof(SP_DEVINFO_DATA);
	if (!pSetupDiEnumDeviceInfo(*dev_info, _index, dev_info_data)) {
		if (GetLastError() != ERROR_NO_MORE_ITEMS) {
			usbi_err(ctx, "Could not obtain device info data for index %u: %s",
				_index, windows_error_str(0));
		}
		pSetupDiDestroyDeviceInfoList(*dev_info);
		*dev_info = INVALID_HANDLE_VALUE;
		return false;
	}
	return true;
}
