static int cac_pin_cmd(sc_card_t *card, struct sc_pin_cmd_data *data, int *tries_left)
{
	 
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();

	if (data->cmd == SC_PIN_CMD_CHANGE) {
		int i = 0;
		if (data->pin2.len < 6) {
			return SC_ERROR_INVALID_PIN_LENGTH;
		}
		for(i=0; i < data->pin2.len; ++i) {
			if (!isdigit(data->pin2.data[i])) {
				return SC_ERROR_INVALID_DATA;
			}
		}
	}

	return  iso_drv->ops->pin_cmd(card, data, tries_left);
}