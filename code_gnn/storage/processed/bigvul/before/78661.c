static struct sc_card_driver * sc_get_driver(void)
{
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();

	piv_ops = *iso_drv->ops;
	piv_ops.match_card = piv_match_card;
	piv_ops.init = piv_init;
	piv_ops.finish = piv_finish;

	piv_ops.select_file =  piv_select_file;  
	piv_ops.get_challenge = piv_get_challenge;
	piv_ops.logout = piv_logout;
	piv_ops.read_binary = piv_read_binary;
	piv_ops.write_binary = piv_write_binary;
	piv_ops.set_security_env = piv_set_security_env;
	piv_ops.restore_security_env = piv_restore_security_env;
	piv_ops.compute_signature = piv_compute_signature;
	piv_ops.decipher =  piv_decipher;
	piv_ops.check_sw = piv_check_sw;
	piv_ops.card_ctl = piv_card_ctl;
	piv_ops.pin_cmd = piv_pin_cmd;
	piv_ops.card_reader_lock_obtained = piv_card_reader_lock_obtained;

	return &piv_drv;
}