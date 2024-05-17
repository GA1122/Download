static int dw2102_frontend_attach(struct dvb_usb_adapter *d)
{
	if (dw2102_properties.i2c_algo == &dw2102_serit_i2c_algo) {
		 
		d->fe_adap[0].fe = dvb_attach(si21xx_attach, &serit_sp1511lhb_config,
					&d->dev->i2c_adap);
		if (d->fe_adap[0].fe != NULL) {
			d->fe_adap[0].fe->ops.set_voltage = dw210x_set_voltage;
			info("Attached si21xx!");
			return 0;
		}
	}

	if (dw2102_properties.i2c_algo == &dw2102_earda_i2c_algo) {
		d->fe_adap[0].fe = dvb_attach(stv0288_attach, &earda_config,
					&d->dev->i2c_adap);
		if (d->fe_adap[0].fe != NULL) {
			if (dvb_attach(stb6000_attach, d->fe_adap[0].fe, 0x61,
					&d->dev->i2c_adap)) {
				d->fe_adap[0].fe->ops.set_voltage = dw210x_set_voltage;
				info("Attached stv0288!");
				return 0;
			}
		}
	}

	if (dw2102_properties.i2c_algo == &dw2102_i2c_algo) {
		 
		d->fe_adap[0].fe = dvb_attach(stv0299_attach, &sharp_z0194a_config,
					&d->dev->i2c_adap);
		if (d->fe_adap[0].fe != NULL) {
			d->fe_adap[0].fe->ops.set_voltage = dw210x_set_voltage;
			info("Attached stv0299!");
			return 0;
		}
	}
	return -EIO;
}