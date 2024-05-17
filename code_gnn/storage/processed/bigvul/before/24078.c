static int setflashmode (struct airo_info *ai) {
	set_bit (FLAG_FLASHING, &ai->flags);

	OUT4500(ai, SWS0, FLASH_COMMAND);
	OUT4500(ai, SWS1, FLASH_COMMAND);
	if (probe) {
		OUT4500(ai, SWS0, FLASH_COMMAND);
		OUT4500(ai, COMMAND,0x10);
	} else {
		OUT4500(ai, SWS2, FLASH_COMMAND);
		OUT4500(ai, SWS3, FLASH_COMMAND);
		OUT4500(ai, COMMAND,0);
	}
	msleep(500);		 

	if(!waitbusy(ai)) {
		clear_bit (FLAG_FLASHING, &ai->flags);
		airo_print_info(ai->dev->name, "Waitbusy hang after setflash mode");
		return -EIO;
	}
	return 0;
}