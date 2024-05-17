static void powermate_sync_state(struct powermate_device *pm)
{
	if (pm->requires_update == 0)
		return;  
	if (pm->config->status == -EINPROGRESS)
		return;  

	if (pm->requires_update & UPDATE_PULSE_ASLEEP){
		pm->configcr->wValue = cpu_to_le16( SET_PULSE_ASLEEP );
		pm->configcr->wIndex = cpu_to_le16( pm->pulse_asleep ? 1 : 0 );
		pm->requires_update &= ~UPDATE_PULSE_ASLEEP;
	}else if (pm->requires_update & UPDATE_PULSE_AWAKE){
		pm->configcr->wValue = cpu_to_le16( SET_PULSE_AWAKE );
		pm->configcr->wIndex = cpu_to_le16( pm->pulse_awake ? 1 : 0 );
		pm->requires_update &= ~UPDATE_PULSE_AWAKE;
	}else if (pm->requires_update & UPDATE_PULSE_MODE){
		int op, arg;
		 
		if (pm->pulse_speed < 255) {
			op = 0;                    
			arg = 255 - pm->pulse_speed;
		} else if (pm->pulse_speed > 255) {
			op = 2;                    
			arg = pm->pulse_speed - 255;
		} else {
			op = 1;                    
			arg = 0;                   
		}
		pm->configcr->wValue = cpu_to_le16( (pm->pulse_table << 8) | SET_PULSE_MODE );
		pm->configcr->wIndex = cpu_to_le16( (arg << 8) | op );
		pm->requires_update &= ~UPDATE_PULSE_MODE;
	} else if (pm->requires_update & UPDATE_STATIC_BRIGHTNESS) {
		pm->configcr->wValue = cpu_to_le16( SET_STATIC_BRIGHTNESS );
		pm->configcr->wIndex = cpu_to_le16( pm->static_brightness );
		pm->requires_update &= ~UPDATE_STATIC_BRIGHTNESS;
	} else {
		printk(KERN_ERR "powermate: unknown update required");
		pm->requires_update = 0;  
		return;
	}

 

	pm->configcr->bRequestType = 0x41;  
	pm->configcr->bRequest = 0x01;
	pm->configcr->wLength = 0;

	usb_fill_control_urb(pm->config, pm->udev, usb_sndctrlpipe(pm->udev, 0),
			     (void *) pm->configcr, NULL, 0,
			     powermate_config_complete, pm);

	if (usb_submit_urb(pm->config, GFP_ATOMIC))
		printk(KERN_ERR "powermate: usb_submit_urb(config) failed");
}
