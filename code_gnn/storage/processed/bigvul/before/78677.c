struct sc_card_driver * sc_get_rtecp_driver(void)
{
	if (iso_ops == NULL)
		iso_ops = sc_get_iso7816_driver()->ops;
	rtecp_ops = *iso_ops;

	rtecp_ops.match_card = rtecp_match_card;
	rtecp_ops.init = rtecp_init;
	 
	rtecp_ops.write_binary = NULL;
	 
	rtecp_ops.read_record = NULL;
	rtecp_ops.write_record = NULL;
	rtecp_ops.append_record = NULL;
	rtecp_ops.update_record = NULL;
	rtecp_ops.select_file = rtecp_select_file;
	rtecp_ops.get_response = NULL;
	 
	rtecp_ops.verify = rtecp_verify;
	rtecp_ops.logout = rtecp_logout;
	 
	 
	rtecp_ops.decipher = rtecp_decipher;
	rtecp_ops.compute_signature = rtecp_compute_signature;
	rtecp_ops.change_reference_data = rtecp_change_reference_data;
	rtecp_ops.reset_retry_counter = rtecp_reset_retry_counter;
	rtecp_ops.create_file = rtecp_create_file;
	 
	rtecp_ops.list_files = rtecp_list_files;
	 
	rtecp_ops.card_ctl = rtecp_card_ctl;
	 
	rtecp_ops.construct_fci = rtecp_construct_fci;
	rtecp_ops.pin_cmd = NULL;

	return &rtecp_drv;
}
