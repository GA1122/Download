static void ims_pcu_destroy_bootloader_mode(struct ims_pcu *pcu)
{
	 
	wait_for_completion(&pcu->async_firmware_done);
}
