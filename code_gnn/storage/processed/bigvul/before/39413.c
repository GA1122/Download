static void recal_interrupt(void)
{
	debugt(__func__, "");
	if (inr != 2)
		FDCS->reset = 1;
	else if (ST0 & ST0_ECE) {
		switch (DRS->track) {
		case NEED_1_RECAL:
			debugt(__func__, "need 1 recal");
			 
			cont->error();
			cont->redo();
			return;
		case NEED_2_RECAL:
			debugt(__func__, "need 2 recal");
			 
			debug_dcl(DP->flags,
				  "clearing NEWCHANGE flag because of second recalibrate\n");

			clear_bit(FD_DISK_NEWCHANGE_BIT, &DRS->flags);
			DRS->select_date = jiffies;
			 
		default:
			debugt(__func__, "default");
			 
			DRS->track = NEED_1_RECAL;
			break;
		}
	} else
		DRS->track = ST1;
	floppy_ready();
}
