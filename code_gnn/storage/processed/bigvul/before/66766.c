static int dw2104_i2c_transfer(struct i2c_adapter *adap, struct i2c_msg msg[], int num)
{
	struct dvb_usb_device *d = i2c_get_adapdata(adap);
	int len, i, j, ret;

	if (!d)
		return -ENODEV;
	if (mutex_lock_interruptible(&d->i2c_mutex) < 0)
		return -EAGAIN;

	for (j = 0; j < num; j++) {
		switch (msg[j].addr) {
		case(DW2102_RC_QUERY): {
			u8 ibuf[2];
			dw210x_op_rw(d->udev, 0xb8, 0, 0,
					ibuf, 2, DW210X_READ_MSG);
			memcpy(msg[j].buf, ibuf , 2);
			break;
		}
		case(DW2102_VOLTAGE_CTRL): {
			u8 obuf[2];
			obuf[0] = 0x30;
			obuf[1] = msg[j].buf[0];
			dw210x_op_rw(d->udev, 0xb2, 0, 0,
					obuf, 2, DW210X_WRITE_MSG);
			break;
		}
		 
		default: {
			if (msg[j].flags == I2C_M_RD) {
				 
				u8  ibuf[MAX_XFER_SIZE];

				if (2 + msg[j].len > sizeof(ibuf)) {
					warn("i2c rd: len=%d is too big!\n",
					     msg[j].len);
					ret = -EOPNOTSUPP;
					goto unlock;
				}

				dw210x_op_rw(d->udev, 0xc3,
						(msg[j].addr << 1) + 1, 0,
						ibuf, msg[j].len + 2,
						DW210X_READ_MSG);
				memcpy(msg[j].buf, ibuf + 2, msg[j].len);
				mdelay(10);
			} else if (((msg[j].buf[0] == 0xb0) &&
						(msg[j].addr == 0x68)) ||
						((msg[j].buf[0] == 0xf7) &&
						(msg[j].addr == 0x55))) {
				 
				u8 obuf[19];
				obuf[0] = msg[j].addr << 1;
				obuf[1] = (msg[j].len > 15 ? 17 : msg[j].len);
				obuf[2] = msg[j].buf[0];
				len = msg[j].len - 1;
				i = 1;
				do {
					memcpy(obuf + 3, msg[j].buf + i,
							(len > 16 ? 16 : len));
					dw210x_op_rw(d->udev, 0xc2, 0, 0,
						obuf, (len > 16 ? 16 : len) + 3,
						DW210X_WRITE_MSG);
					i += 16;
					len -= 16;
				} while (len > 0);
			} else {
				 
				u8 obuf[MAX_XFER_SIZE];

				if (2 + msg[j].len > sizeof(obuf)) {
					warn("i2c wr: len=%d is too big!\n",
					     msg[j].len);
					ret = -EOPNOTSUPP;
					goto unlock;
				}

				obuf[0] = msg[j].addr << 1;
				obuf[1] = msg[j].len;
				memcpy(obuf + 2, msg[j].buf, msg[j].len);
				dw210x_op_rw(d->udev, 0xc2, 0, 0,
						obuf, msg[j].len + 2,
						DW210X_WRITE_MSG);
			}
			break;
		}
		}

	}
	ret = num;

unlock:
	mutex_unlock(&d->i2c_mutex);
	return ret;
}