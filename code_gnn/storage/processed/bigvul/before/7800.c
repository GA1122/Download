static int h2c_handle_settings(struct h2c *h2c)
{
	unsigned int offset;
	int error;

	if (h2c->dff & H2_F_SETTINGS_ACK) {
		if (h2c->dfl) {
			error = H2_ERR_FRAME_SIZE_ERROR;
			goto fail;
		}
		return 1;
	}

	if (h2c->dsi != 0) {
		error = H2_ERR_PROTOCOL_ERROR;
		goto fail;
	}

	if (h2c->dfl % 6) {
		error = H2_ERR_FRAME_SIZE_ERROR;
		goto fail;
	}

	 
	if (h2c->dfl > global.tune.bufsize) {
		error = H2_ERR_FRAME_SIZE_ERROR;
		goto fail;
	}

	 
	if (h2c->dbuf->i < h2c->dfl)
		return 0;

	 
	for (offset = 0; offset < h2c->dfl; offset += 6) {
		uint16_t type = h2_get_n16(h2c->dbuf, offset);
		int32_t  arg  = h2_get_n32(h2c->dbuf, offset + 2);

		switch (type) {
		case H2_SETTINGS_INITIAL_WINDOW_SIZE:
			 
			if (arg < 0) {  
				error = H2_ERR_FLOW_CONTROL_ERROR;
				goto fail;
			}
			h2c_update_all_ws(h2c, arg - h2c->miw);
			h2c->miw = arg;
			break;
		case H2_SETTINGS_MAX_FRAME_SIZE:
			if (arg < 16384 || arg > 16777215) {  
				error = H2_ERR_PROTOCOL_ERROR;
				goto fail;
			}
			h2c->mfs = arg;
			break;
		case H2_SETTINGS_ENABLE_PUSH:
			if (arg < 0 || arg > 1) {  
				error = H2_ERR_PROTOCOL_ERROR;
				goto fail;
			}
			break;
		}
	}

	 
	h2c->st0 = H2_CS_FRAME_A;
	return 1;
 fail:
	h2c_error(h2c, error);
	return 0;
}
