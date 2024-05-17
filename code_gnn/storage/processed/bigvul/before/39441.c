static int start_motor(void (*function)(void))
{
	int mask;
	int data;

	mask = 0xfc;
	data = UNIT(current_drive);
	if (!(raw_cmd->flags & FD_RAW_NO_MOTOR)) {
		if (!(FDCS->dor & (0x10 << UNIT(current_drive)))) {
			set_debugt();
			 
			DRS->first_read_date = 0;
			 
			DRS->spinup_date = jiffies;
			data |= (0x10 << UNIT(current_drive));
		}
	} else if (FDCS->dor & (0x10 << UNIT(current_drive)))
		mask &= ~(0x10 << UNIT(current_drive));

	 
	del_timer(motor_off_timer + current_drive);
	set_dor(fdc, mask, data);

	 
	return fd_wait_for_completion(DRS->select_date + DP->select_delay,
				      function);
}
