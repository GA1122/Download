static struct sc_card_driver * sc_get_driver(void)
{
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();

	if (iso_ops == NULL)
		iso_ops = iso_drv->ops;
  
	entersafe_ops = *iso_drv->ops;
	entersafe_ops.match_card = entersafe_match_card;
	entersafe_ops.init   = entersafe_init;
	entersafe_ops.read_binary = entersafe_read_binary;
	entersafe_ops.write_binary = NULL;
	entersafe_ops.update_binary = entersafe_update_binary;
	entersafe_ops.select_file = entersafe_select_file;
	entersafe_ops.restore_security_env = entersafe_restore_security_env;
	entersafe_ops.set_security_env  = entersafe_set_security_env;
	entersafe_ops.decipher = entersafe_decipher;
	entersafe_ops.compute_signature = entersafe_compute_signature;
	entersafe_ops.create_file = entersafe_create_file;
	entersafe_ops.delete_file = NULL;
	entersafe_ops.pin_cmd = entersafe_pin_cmd;
	entersafe_ops.card_ctl    = entersafe_card_ctl_2048;
	entersafe_ops.process_fci = entersafe_process_fci;
	return &entersafe_drv;
}
