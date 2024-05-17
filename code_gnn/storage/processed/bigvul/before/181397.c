 static int su3000_frontend_attach(struct dvb_usb_adapter *d)
// static int su3000_frontend_attach(struct dvb_usb_adapter *adap)
  {
	u8 obuf[3] = { 0xe, 0x80, 0 };
	u8 ibuf[] = { 0 };
// 	struct dvb_usb_device *d = adap->dev;
// 	struct dw2102_state *state = d->priv;
// 
// 	mutex_lock(&d->data_mutex);
// 
// 	state->data[0] = 0xe;
// 	state->data[1] = 0x80;
// 	state->data[2] = 0;
  
	if (dvb_usb_generic_rw(d->dev, obuf, 3, ibuf, 1, 0) < 0)
// 	if (dvb_usb_generic_rw(d, state->data, 3, state->data, 1, 0) < 0)
  		err("command 0x0e transfer failed.");
  
	obuf[0] = 0xe;
	obuf[1] = 0x02;
	obuf[2] = 1;
// 	state->data[0] = 0xe;
// 	state->data[1] = 0x02;
// 	state->data[2] = 1;
  
	if (dvb_usb_generic_rw(d->dev, obuf, 3, ibuf, 1, 0) < 0)
// 	if (dvb_usb_generic_rw(d, state->data, 3, state->data, 1, 0) < 0)
  		err("command 0x0e transfer failed.");
  	msleep(300);
  
	obuf[0] = 0xe;
	obuf[1] = 0x83;
	obuf[2] = 0;
// 	state->data[0] = 0xe;
// 	state->data[1] = 0x83;
// 	state->data[2] = 0;
  
	if (dvb_usb_generic_rw(d->dev, obuf, 3, ibuf, 1, 0) < 0)
// 	if (dvb_usb_generic_rw(d, state->data, 3, state->data, 1, 0) < 0)
  		err("command 0x0e transfer failed.");
  
	obuf[0] = 0xe;
	obuf[1] = 0x83;
	obuf[2] = 1;
// 	state->data[0] = 0xe;
// 	state->data[1] = 0x83;
// 	state->data[2] = 1;
  
	if (dvb_usb_generic_rw(d->dev, obuf, 3, ibuf, 1, 0) < 0)
// 	if (dvb_usb_generic_rw(d, state->data, 3, state->data, 1, 0) < 0)
  		err("command 0x0e transfer failed.");
  
	obuf[0] = 0x51;
// 	state->data[0] = 0x51;
  
	if (dvb_usb_generic_rw(d->dev, obuf, 1, ibuf, 1, 0) < 0)
// 	if (dvb_usb_generic_rw(d, state->data, 1, state->data, 1, 0) < 0)
  		err("command 0x51 transfer failed.");
  
	d->fe_adap[0].fe = dvb_attach(ds3000_attach, &su3000_ds3000_config,
					&d->dev->i2c_adap);
	if (d->fe_adap[0].fe == NULL)
// 	mutex_unlock(&d->data_mutex);
// 
// 	adap->fe_adap[0].fe = dvb_attach(ds3000_attach, &su3000_ds3000_config,
// 					&d->i2c_adap);
// 	if (adap->fe_adap[0].fe == NULL)
  		return -EIO;
  
	if (dvb_attach(ts2020_attach, d->fe_adap[0].fe,
// 	if (dvb_attach(ts2020_attach, adap->fe_adap[0].fe,
  				&dw2104_ts2020_config,
				&d->dev->i2c_adap)) {
// 				&d->i2c_adap)) {
  		info("Attached DS3000/TS2020!");
  		return 0;
  	}
 
 	info("Failed to attach DS3000/TS2020!");
  	return -EIO;
  }