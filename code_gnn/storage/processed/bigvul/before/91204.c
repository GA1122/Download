static void __get_guid(struct ipmi_smi *intf)
{
	int rv;
	struct bmc_device *bmc = intf->bmc;

	bmc->dyn_guid_set = 2;
	intf->null_user_handler = guid_handler;
	rv = send_guid_cmd(intf, 0);
	if (rv)
		 
		bmc->dyn_guid_set = 0;

	wait_event(intf->waitq, bmc->dyn_guid_set != 2);

	 
	smp_rmb();

	intf->null_user_handler = NULL;
}
