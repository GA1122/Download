static int bap_setup(struct airo_info *ai, u16 rid, u16 offset, int whichbap )
{
	int timeout = 50;
	int max_tries = 3;

	OUT4500(ai, SELECT0+whichbap, rid);
	OUT4500(ai, OFFSET0+whichbap, offset);
	while (1) {
		int status = IN4500(ai, OFFSET0+whichbap);
		if (status & BAP_BUSY) {
                         
			if (timeout--) {
				continue;
			}
		} else if ( status & BAP_ERR ) {
			 
			airo_print_err(ai->dev->name, "BAP error %x %d",
				status, whichbap );
			return ERROR;
		} else if (status & BAP_DONE) {  
			return SUCCESS;
		}
		if ( !(max_tries--) ) {
			airo_print_err(ai->dev->name,
				"BAP setup error too many retries\n");
			return ERROR;
		}
		OUT4500(ai, SELECT0+whichbap, rid);
		OUT4500(ai, OFFSET0+whichbap, offset);
		timeout = 50;
	}
}
