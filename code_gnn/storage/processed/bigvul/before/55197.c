static void gtco_urb_callback(struct urb *urbinfo)
{
	struct gtco *device = urbinfo->context;
	struct input_dev  *inputdev;
	int               rc;
	u32               val = 0;
	char              le_buffer[2];

	inputdev = device->inputdevice;

	 
	if (urbinfo->status == -ECONNRESET ||
	    urbinfo->status == -ENOENT ||
	    urbinfo->status == -ESHUTDOWN) {

		 
		return;
	}

	if (urbinfo->status != 0) {
		 
		goto resubmit;
	}

	 

	 
	if (inputdev->id.product == PID_1000 ||
	    inputdev->id.product == PID_1001 ||
	    inputdev->id.product == PID_1002) {

		 
		switch (device->buffer[0]) {
		case 5:
			 
			val = ((u16)(device->buffer[8]) << 1);
			val |= (u16)(device->buffer[7] >> 7);
			input_report_abs(inputdev, ABS_PRESSURE,
					 device->buffer[8]);

			 
			device->buffer[7] = (u8)((device->buffer[7]) & 0x7F);

			 
		case 4:
			 
			input_report_abs(inputdev, ABS_TILT_X,
					 sign_extend32(device->buffer[6], 6));

			input_report_abs(inputdev, ABS_TILT_Y,
					 sign_extend32(device->buffer[7], 6));

			 
		case 2:
		case 3:
			 
			val = (device->buffer[5]) & MASK_BUTTON;

			 
			input_event(inputdev, EV_MSC, MSC_SERIAL, val);

			 
		case 1:
			 
			val = get_unaligned_le16(&device->buffer[1]);
			input_report_abs(inputdev, ABS_X, val);

			val = get_unaligned_le16(&device->buffer[3]);
			input_report_abs(inputdev, ABS_Y, val);

			 
			val = device->buffer[5] & MASK_INRANGE ? 1 : 0;
			input_report_abs(inputdev, ABS_DISTANCE, val);

			 
			 
			if (device->buffer[0] == 1) {

				 
				val = device->buffer[5] & MASK_BUTTON;
				dev_dbg(&device->intf->dev,
					"======>>>>>>REPORT 1: val 0x%X(%d)\n",
					val, val);

				 
				input_event(inputdev, EV_MSC, MSC_SERIAL, val);
			}
			break;

		case 7:
			 
			input_event(inputdev, EV_MSC, MSC_SCAN,
				    device->buffer[1]);
			break;
		}
	}

	 
	if (inputdev->id.product == PID_400 ||
	    inputdev->id.product == PID_401) {

		 
		if (device->buffer[0] == 2) {
			 
			input_event(inputdev, EV_MSC, MSC_SCAN, device->buffer[1]);
		}

		 
		if (device->buffer[0] == 1) {
			char buttonbyte;

			 
			if (device->max_X > 0x10000) {

				val = (u16)(((u16)(device->buffer[2] << 8)) | (u8)device->buffer[1]);
				val |= (u32)(((u8)device->buffer[3] & 0x1) << 16);

				input_report_abs(inputdev, ABS_X, val);

				le_buffer[0]  = (u8)((u8)(device->buffer[3]) >> 1);
				le_buffer[0] |= (u8)((device->buffer[3] & 0x1) << 7);

				le_buffer[1]  = (u8)(device->buffer[4] >> 1);
				le_buffer[1] |= (u8)((device->buffer[5] & 0x1) << 7);

				val = get_unaligned_le16(le_buffer);
				input_report_abs(inputdev, ABS_Y, val);

				 
				buttonbyte = device->buffer[5] >> 1;
			} else {

				val = get_unaligned_le16(&device->buffer[1]);
				input_report_abs(inputdev, ABS_X, val);

				val = get_unaligned_le16(&device->buffer[3]);
				input_report_abs(inputdev, ABS_Y, val);

				buttonbyte = device->buffer[5];
			}

			 
			val = buttonbyte & MASK_INRANGE ? 1 : 0;
			input_report_abs(inputdev, ABS_DISTANCE, val);

			 
			val = buttonbyte & 0x0F;
#ifdef USE_BUTTONS
			for (i = 0; i < 5; i++)
				input_report_key(inputdev, BTN_DIGI + i, val & (1 << i));
#else
			 
			input_event(inputdev, EV_MSC, MSC_SERIAL, val);
#endif

			 
			input_report_abs(inputdev, ABS_MISC, device->buffer[6]);
		}
	}

	 
	input_event(inputdev, EV_MSC, MSC_RAW,  device->buffer[0]);

	 
	input_sync(inputdev);

 resubmit:
	rc = usb_submit_urb(urbinfo, GFP_ATOMIC);
	if (rc != 0)
		dev_err(&device->intf->dev,
			"usb_submit_urb failed rc=0x%x\n", rc);
}
