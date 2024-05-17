 static int su3000_i2c_transfer(struct i2c_adapter *adap, struct i2c_msg msg[],
  								int num)
  {
  	struct dvb_usb_device *d = i2c_get_adapdata(adap);
	u8 obuf[0x40], ibuf[0x40];
// 	struct dw2102_state *state;
  
  	if (!d)
  		return -ENODEV;
// 
// 	state = d->priv;
// 
  	if (mutex_lock_interruptible(&d->i2c_mutex) < 0)
  		return -EAGAIN;
// 	if (mutex_lock_interruptible(&d->data_mutex) < 0) {
// 		mutex_unlock(&d->i2c_mutex);
// 		return -EAGAIN;
// 	}
  
  	switch (num) {
  	case 1:
  		switch (msg[0].addr) {
  		case SU3000_STREAM_CTRL:
			obuf[0] = msg[0].buf[0] + 0x36;
			obuf[1] = 3;
			obuf[2] = 0;
			if (dvb_usb_generic_rw(d, obuf, 3, ibuf, 0, 0) < 0)
// 			state->data[0] = msg[0].buf[0] + 0x36;
// 			state->data[1] = 3;
// 			state->data[2] = 0;
// 			if (dvb_usb_generic_rw(d, state->data, 3,
// 					state->data, 0, 0) < 0)
  				err("i2c transfer failed.");
  			break;
  		case DW2102_RC_QUERY:
			obuf[0] = 0x10;
			if (dvb_usb_generic_rw(d, obuf, 1, ibuf, 2, 0) < 0)
// 			state->data[0] = 0x10;
// 			if (dvb_usb_generic_rw(d, state->data, 1,
// 					state->data, 2, 0) < 0)
  				err("i2c transfer failed.");
			msg[0].buf[1] = ibuf[0];
			msg[0].buf[0] = ibuf[1];
// 			msg[0].buf[1] = state->data[0];
// 			msg[0].buf[0] = state->data[1];
  			break;
  		default:
  			 
			obuf[0] = 0x08;
			obuf[1] = msg[0].addr;
			obuf[2] = msg[0].len;
// 			state->data[0] = 0x08;
// 			state->data[1] = msg[0].addr;
// 			state->data[2] = msg[0].len;
  
			memcpy(&obuf[3], msg[0].buf, msg[0].len);
// 			memcpy(&state->data[3], msg[0].buf, msg[0].len);
  
			if (dvb_usb_generic_rw(d, obuf, msg[0].len + 3,
						ibuf, 1, 0) < 0)
// 			if (dvb_usb_generic_rw(d, state->data, msg[0].len + 3,
// 						state->data, 1, 0) < 0)
  				err("i2c transfer failed.");
  
  		}
  		break;
  	case 2:
  		 
		obuf[0] = 0x09;
		obuf[1] = msg[0].len;
		obuf[2] = msg[1].len;
		obuf[3] = msg[0].addr;
		memcpy(&obuf[4], msg[0].buf, msg[0].len);
		if (dvb_usb_generic_rw(d, obuf, msg[0].len + 4,
					ibuf, msg[1].len + 1, 0) < 0)
// 		state->data[0] = 0x09;
// 		state->data[1] = msg[0].len;
// 		state->data[2] = msg[1].len;
// 		state->data[3] = msg[0].addr;
// 		memcpy(&state->data[4], msg[0].buf, msg[0].len);
// 
// 		if (dvb_usb_generic_rw(d, state->data, msg[0].len + 4,
// 					state->data, msg[1].len + 1, 0) < 0)
  			err("i2c transfer failed.");
  
		memcpy(msg[1].buf, &ibuf[1], msg[1].len);
// 		memcpy(msg[1].buf, &state->data[1], msg[1].len);
  		break;
  	default:
  		warn("more than 2 i2c messages at a time is not handled yet.");
  		break;
  	}
// 	mutex_unlock(&d->data_mutex);
  	mutex_unlock(&d->i2c_mutex);
  	return num;
  }