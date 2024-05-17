sc_get_driver(void)
{
	if (iso_ops == NULL)
		iso_ops = sc_get_iso7816_driver()->ops;

	auth_ops = *iso_ops;
	auth_ops.match_card = auth_match_card;
	auth_ops.init = auth_init;
	auth_ops.finish = auth_finish;
	auth_ops.select_file = auth_select_file;
	auth_ops.list_files = auth_list_files;
	auth_ops.delete_file = auth_delete_file;
	auth_ops.create_file = auth_create_file;
	auth_ops.read_binary = auth_read_binary;
	auth_ops.update_binary = auth_update_binary;
	auth_ops.read_record = auth_read_record;
	auth_ops.delete_record = auth_delete_record;
	auth_ops.card_ctl = auth_card_ctl;
	auth_ops.set_security_env = auth_set_security_env;
	auth_ops.restore_security_env = auth_restore_security_env;
	auth_ops.compute_signature = auth_compute_signature;
	auth_ops.decipher = auth_decipher;
	auth_ops.process_fci = auth_process_fci;
	auth_ops.pin_cmd = auth_pin_cmd;
	auth_ops.logout = auth_logout;
	auth_ops.check_sw = auth_check_sw;
	return &auth_drv;
}
