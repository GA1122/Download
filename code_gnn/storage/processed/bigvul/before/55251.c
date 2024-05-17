static bool is_marantz_denon_dac(unsigned int id)
{
	switch (id) {
	case USB_ID(0x154e, 0x1003):  
	case USB_ID(0x154e, 0x3005):  
	case USB_ID(0x154e, 0x3006):  
		return true;
	}
	return false;
}
