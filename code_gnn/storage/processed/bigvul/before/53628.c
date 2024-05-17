u8 cdc_ncm_select_altsetting(struct usb_interface *intf)
{
	struct usb_host_interface *alt;

	 
	if (intf->num_altsetting < 2)
		return intf->cur_altsetting->desc.bAlternateSetting;

	if (prefer_mbim) {
		alt = usb_altnum_to_altsetting(intf, CDC_NCM_COMM_ALTSETTING_MBIM);
		if (alt && cdc_ncm_comm_intf_is_mbim(alt))
			return CDC_NCM_COMM_ALTSETTING_MBIM;
	}
	return CDC_NCM_COMM_ALTSETTING_NCM;
}
