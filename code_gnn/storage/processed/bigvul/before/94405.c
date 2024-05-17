sd_read_capacity(struct scsi_disk *sdkp, unsigned char *buffer)
{
	int sector_size;
	struct scsi_device *sdp = sdkp->device;
	sector_t old_capacity = sdkp->capacity;

	if (sd_try_rc16_first(sdp)) {
		sector_size = read_capacity_16(sdkp, sdp, buffer);
		if (sector_size == -EOVERFLOW)
			goto got_data;
		if (sector_size == -ENODEV)
			return;
		if (sector_size < 0)
			sector_size = read_capacity_10(sdkp, sdp, buffer);
		if (sector_size < 0)
			return;
	} else {
		sector_size = read_capacity_10(sdkp, sdp, buffer);
		if (sector_size == -EOVERFLOW)
			goto got_data;
		if (sector_size < 0)
			return;
		if ((sizeof(sdkp->capacity) > 4) &&
		    (sdkp->capacity > 0xffffffffULL)) {
			int old_sector_size = sector_size;
			sd_printk(KERN_NOTICE, sdkp, "Very big device. "
					"Trying to use READ CAPACITY(16).\n");
			sector_size = read_capacity_16(sdkp, sdp, buffer);
			if (sector_size < 0) {
				sd_printk(KERN_NOTICE, sdkp,
					"Using 0xffffffff as device size\n");
				sdkp->capacity = 1 + (sector_t) 0xffffffff;
				sector_size = old_sector_size;
				goto got_data;
			}
		}
	}

	 
	if (sdp->fix_capacity ||
	    (sdp->guess_capacity && (sdkp->capacity & 0x01))) {
		sd_printk(KERN_INFO, sdkp, "Adjusting the sector count "
				"from its reported value: %llu\n",
				(unsigned long long) sdkp->capacity);
		--sdkp->capacity;
	}

got_data:
	if (sector_size == 0) {
		sector_size = 512;
		sd_printk(KERN_NOTICE, sdkp, "Sector size 0 reported, "
			  "assuming 512.\n");
	}

	if (sector_size != 512 &&
	    sector_size != 1024 &&
	    sector_size != 2048 &&
	    sector_size != 4096 &&
	    sector_size != 256) {
		sd_printk(KERN_NOTICE, sdkp, "Unsupported sector size %d.\n",
			  sector_size);
		 
		sdkp->capacity = 0;
		 
		sector_size = 512;
	}
	blk_queue_logical_block_size(sdp->request_queue, sector_size);

	{
		char cap_str_2[10], cap_str_10[10];
		u64 sz = (u64)sdkp->capacity << ilog2(sector_size);

		string_get_size(sz, STRING_UNITS_2, cap_str_2,
				sizeof(cap_str_2));
		string_get_size(sz, STRING_UNITS_10, cap_str_10,
				sizeof(cap_str_10));

		if (sdkp->first_scan || old_capacity != sdkp->capacity) {
			sd_printk(KERN_NOTICE, sdkp,
				  "%llu %d-byte logical blocks: (%s/%s)\n",
				  (unsigned long long)sdkp->capacity,
				  sector_size, cap_str_10, cap_str_2);

			if (sdkp->physical_block_size != sector_size)
				sd_printk(KERN_NOTICE, sdkp,
					  "%u-byte physical blocks\n",
					  sdkp->physical_block_size);
		}
	}

	 
	if (sector_size == 4096)
		sdkp->capacity <<= 3;
	else if (sector_size == 2048)
		sdkp->capacity <<= 2;
	else if (sector_size == 1024)
		sdkp->capacity <<= 1;
	else if (sector_size == 256)
		sdkp->capacity >>= 1;

	blk_queue_physical_block_size(sdp->request_queue,
				      sdkp->physical_block_size);
	sdkp->device->sector_size = sector_size;
}