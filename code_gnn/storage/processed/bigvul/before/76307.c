int open_for_data(struct cdrom_device_info *cdi)
{
	int ret;
	const struct cdrom_device_ops *cdo = cdi->ops;
	tracktype tracks;
	cd_dbg(CD_OPEN, "entering open_for_data\n");
	 
	if (cdo->drive_status != NULL) {
		ret = cdo->drive_status(cdi, CDSL_CURRENT);
		cd_dbg(CD_OPEN, "drive_status=%d\n", ret);
		if (ret == CDS_TRAY_OPEN) {
			cd_dbg(CD_OPEN, "the tray is open...\n");
			 
			if (CDROM_CAN(CDC_CLOSE_TRAY) &&
			    cdi->options & CDO_AUTO_CLOSE) {
				cd_dbg(CD_OPEN, "trying to close the tray\n");
				ret=cdo->tray_move(cdi,0);
				if (ret) {
					cd_dbg(CD_OPEN, "bummer. tried to close the tray but failed.\n");
					 
					ret=-ENOMEDIUM;
					goto clean_up_and_return;
				}
			} else {
				cd_dbg(CD_OPEN, "bummer. this drive can't close the tray.\n");
				ret=-ENOMEDIUM;
				goto clean_up_and_return;
			}
			 
			ret = cdo->drive_status(cdi, CDSL_CURRENT);
			if ((ret == CDS_NO_DISC) || (ret==CDS_TRAY_OPEN)) {
				cd_dbg(CD_OPEN, "bummer. the tray is still not closed.\n");
				cd_dbg(CD_OPEN, "tray might not contain a medium\n");
				ret=-ENOMEDIUM;
				goto clean_up_and_return;
			}
			cd_dbg(CD_OPEN, "the tray is now closed\n");
		}
		 
		ret = cdo->drive_status(cdi, CDSL_CURRENT);
		if (ret!=CDS_DISC_OK) {
			ret = -ENOMEDIUM;
			goto clean_up_and_return;
		}
	}
	cdrom_count_tracks(cdi, &tracks);
	if (tracks.error == CDS_NO_DISC) {
		cd_dbg(CD_OPEN, "bummer. no disc.\n");
		ret=-ENOMEDIUM;
		goto clean_up_and_return;
	}
	 
	if (tracks.data==0) {
		if (cdi->options & CDO_CHECK_TYPE) {
		     
		    cd_dbg(CD_OPEN, "bummer. wrong media type.\n");
		    cd_dbg(CD_WARNING, "pid %d must open device O_NONBLOCK!\n",
			   (unsigned int)task_pid_nr(current));
		    ret=-EMEDIUMTYPE;
		    goto clean_up_and_return;
		}
		else {
		    cd_dbg(CD_OPEN, "wrong media type, but CDO_CHECK_TYPE not set\n");
		}
	}

	cd_dbg(CD_OPEN, "all seems well, opening the devicen");

	 
	ret = cdo->open(cdi, 0);  
	cd_dbg(CD_OPEN, "opening the device gave me %d\n", ret);
	 
	if (ret) {
		cd_dbg(CD_OPEN, "open device failed\n");
		goto clean_up_and_return;
	}
	if (CDROM_CAN(CDC_LOCK) && (cdi->options & CDO_LOCK)) {
			cdo->lock_door(cdi, 1);
			cd_dbg(CD_OPEN, "door locked\n");
	}
	cd_dbg(CD_OPEN, "device opened successfully\n");
	return ret;

	  
clean_up_and_return:
	cd_dbg(CD_OPEN, "open failed\n");
	if (CDROM_CAN(CDC_LOCK) && cdi->options & CDO_LOCK) {
			cdo->lock_door(cdi, 0);
			cd_dbg(CD_OPEN, "door unlocked\n");
	}
	return ret;
}
