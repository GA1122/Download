 static int m88rs2000_frontend_attach(struct dvb_usb_adapter *d)
// static int m88rs2000_frontend_attach(struct dvb_usb_adapter *adap)
  {
	u8 obuf[] = { 0x51 };
	u8 ibuf[] = { 0 };
// 	struct dvb_usb_device *d = adap->dev;
// 	struct dw2102_state *state = d->priv;
// 
// 	mutex_lock(&d->data_mutex);
  
	if (dvb_usb_generic_rw(d->dev, obuf, 1, ibuf, 1, 0) < 0)
// 	state->data[0] = 0x51;
// 
// 	if (dvb_usb_generic_rw(d, state->data, 1, state->data, 1, 0) < 0)
  		err("command 0x51 transfer failed.");
  
	d->fe_adap[0].fe = dvb_attach(m88rs2000_attach, &s421_m88rs2000_config,
					&d->dev->i2c_adap);
// 	mutex_unlock(&d->data_mutex);
  
	if (d->fe_adap[0].fe == NULL)
// 	adap->fe_adap[0].fe = dvb_attach(m88rs2000_attach,
// 					&s421_m88rs2000_config,
// 					&d->i2c_adap);
// 
// 	if (adap->fe_adap[0].fe == NULL)
  		return -EIO;
  
	if (dvb_attach(ts2020_attach, d->fe_adap[0].fe,
// 	if (dvb_attach(ts2020_attach, adap->fe_adap[0].fe,
  				&dw2104_ts2020_config,
				&d->dev->i2c_adap)) {
// 				&d->i2c_adap)) {
  		info("Attached RS2000/TS2020!");
  		return 0;
  	}
 
 	info("Failed to attach RS2000/TS2020!");
 	return -EIO;
 }