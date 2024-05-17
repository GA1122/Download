static int dw3101_i2c_transfer(struct i2c_adapter *adap, struct i2c_msg msg[],
								int num)
{
	struct dvb_usb_device *d = i2c_get_adapdata(adap);
	int ret;
	int i;

	if (!d)
		return -ENODEV;
	if (mutex_lock_interruptible(&d->i2c_mutex) < 0)
		return -EAGAIN;

	switch (num) {
	case 2: {
		 
		 
		u8 ibuf[MAX_XFER_SIZE], obuf[3];

		if (2 + msg[1].len > sizeof(ibuf)) {
			warn("i2c rd: len=%d is too big!\n",
			     msg[1].len);
			ret = -EOPNOTSUPP;
			goto unlock;
		}
		obuf[0] = msg[0].addr << 1;
		obuf[1] = msg[0].len;
		obuf[2] = msg[0].buf[0];
		dw210x_op_rw(d->udev, 0xc2, 0, 0,
				obuf, msg[0].len + 2, DW210X_WRITE_MSG);
		 
		dw210x_op_rw(d->udev, 0xc3, 0x19 , 0,
				ibuf, msg[1].len + 2, DW210X_READ_MSG);
		memcpy(msg[1].buf, ibuf + 2, msg[1].len);

		break;
	}
	case 1:
		switch (msg[0].addr) {
		case 0x60:
		case 0x0c: {
			 
			u8 obuf[MAX_XFER_SIZE];

			if (2 + msg[0].len > sizeof(obuf)) {
				warn("i2c wr: len=%d is too big!\n",
				     msg[0].len);
				ret = -EOPNOTSUPP;
				goto unlock;
			}
			obuf[0] = msg[0].addr << 1;
			obuf[1] = msg[0].len;
			memcpy(obuf + 2, msg[0].buf, msg[0].len);
			dw210x_op_rw(d->udev, 0xc2, 0, 0,
					obuf, msg[0].len + 2, DW210X_WRITE_MSG);
			break;
		}
		case(DW2102_RC_QUERY): {
			u8 ibuf[2];
			dw210x_op_rw(d->udev, 0xb8, 0, 0,
					ibuf, 2, DW210X_READ_MSG);
			memcpy(msg[0].buf, ibuf , 2);
			break;
		}
		}

		break;
	}

	for (i = 0; i < num; i++) {
		deb_xfer("%02x:%02x: %s ", i, msg[i].addr,
				msg[i].flags == 0 ? ">>>" : "<<<");
		debug_dump(msg[i].buf, msg[i].len, deb_xfer);
	}
	ret = num;

unlock:
	mutex_unlock(&d->i2c_mutex);
	return ret;
}