static int sd_prep_fn(struct request_queue *q, struct request *rq)
{
	struct scsi_cmnd *SCpnt;
	struct scsi_device *sdp = q->queuedata;
	struct gendisk *disk = rq->rq_disk;
	struct scsi_disk *sdkp;
	sector_t block = blk_rq_pos(rq);
	sector_t threshold;
	unsigned int this_count = blk_rq_sectors(rq);
	int ret, host_dif;
	unsigned char protect;

	 
	if (rq->cmd_flags & REQ_DISCARD) {
		ret = scsi_setup_discard_cmnd(sdp, rq);
		goto out;
	} else if (rq->cmd_flags & REQ_FLUSH) {
		ret = scsi_setup_flush_cmnd(sdp, rq);
		goto out;
	} else if (rq->cmd_type == REQ_TYPE_BLOCK_PC) {
		ret = scsi_setup_blk_pc_cmnd(sdp, rq);
		goto out;
	} else if (rq->cmd_type != REQ_TYPE_FS) {
		ret = BLKPREP_KILL;
		goto out;
	}
	ret = scsi_setup_fs_cmnd(sdp, rq);
	if (ret != BLKPREP_OK)
		goto out;
	SCpnt = rq->special;
	sdkp = scsi_disk(disk);

	 
	ret = BLKPREP_KILL;

	SCSI_LOG_HLQUEUE(1, scmd_printk(KERN_INFO, SCpnt,
					"sd_init_command: block=%llu, "
					"count=%d\n",
					(unsigned long long)block,
					this_count));

	if (!sdp || !scsi_device_online(sdp) ||
	    block + blk_rq_sectors(rq) > get_capacity(disk)) {
		SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt,
						"Finishing %u sectors\n",
						blk_rq_sectors(rq)));
		SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt,
						"Retry with 0x%p\n", SCpnt));
		goto out;
	}

	if (sdp->changed) {
		 
		 
		goto out;
	}

	 
	threshold = get_capacity(disk) - SD_LAST_BUGGY_SECTORS *
		(sdp->sector_size / 512);

	if (unlikely(sdp->last_sector_bug && block + this_count > threshold)) {
		if (block < threshold) {
			 
			this_count = threshold - block;
		} else {
			 
			this_count = sdp->sector_size / 512;
		}
	}

	SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt, "block=%llu\n",
					(unsigned long long)block));

	 
	if (sdp->sector_size == 1024) {
		if ((block & 1) || (blk_rq_sectors(rq) & 1)) {
			scmd_printk(KERN_ERR, SCpnt,
				    "Bad block number requested\n");
			goto out;
		} else {
			block = block >> 1;
			this_count = this_count >> 1;
		}
	}
	if (sdp->sector_size == 2048) {
		if ((block & 3) || (blk_rq_sectors(rq) & 3)) {
			scmd_printk(KERN_ERR, SCpnt,
				    "Bad block number requested\n");
			goto out;
		} else {
			block = block >> 2;
			this_count = this_count >> 2;
		}
	}
	if (sdp->sector_size == 4096) {
		if ((block & 7) || (blk_rq_sectors(rq) & 7)) {
			scmd_printk(KERN_ERR, SCpnt,
				    "Bad block number requested\n");
			goto out;
		} else {
			block = block >> 3;
			this_count = this_count >> 3;
		}
	}
	if (rq_data_dir(rq) == WRITE) {
		if (!sdp->writeable) {
			goto out;
		}
		SCpnt->cmnd[0] = WRITE_6;
		SCpnt->sc_data_direction = DMA_TO_DEVICE;

		if (blk_integrity_rq(rq) &&
		    sd_dif_prepare(rq, block, sdp->sector_size) == -EIO)
			goto out;

	} else if (rq_data_dir(rq) == READ) {
		SCpnt->cmnd[0] = READ_6;
		SCpnt->sc_data_direction = DMA_FROM_DEVICE;
	} else {
		scmd_printk(KERN_ERR, SCpnt, "Unknown command %x\n", rq->cmd_flags);
		goto out;
	}

	SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt,
					"%s %d/%u 512 byte blocks.\n",
					(rq_data_dir(rq) == WRITE) ?
					"writing" : "reading", this_count,
					blk_rq_sectors(rq)));

	 
	host_dif = scsi_host_dif_capable(sdp->host, sdkp->protection_type);
	if (host_dif)
		protect = 1 << 5;
	else
		protect = 0;

	if (host_dif == SD_DIF_TYPE2_PROTECTION) {
		SCpnt->cmnd = mempool_alloc(sd_cdb_pool, GFP_ATOMIC);

		if (unlikely(SCpnt->cmnd == NULL)) {
			ret = BLKPREP_DEFER;
			goto out;
		}

		SCpnt->cmd_len = SD_EXT_CDB_SIZE;
		memset(SCpnt->cmnd, 0, SCpnt->cmd_len);
		SCpnt->cmnd[0] = VARIABLE_LENGTH_CMD;
		SCpnt->cmnd[7] = 0x18;
		SCpnt->cmnd[9] = (rq_data_dir(rq) == READ) ? READ_32 : WRITE_32;
		SCpnt->cmnd[10] = protect | ((rq->cmd_flags & REQ_FUA) ? 0x8 : 0);

		 
		SCpnt->cmnd[12] = sizeof(block) > 4 ? (unsigned char) (block >> 56) & 0xff : 0;
		SCpnt->cmnd[13] = sizeof(block) > 4 ? (unsigned char) (block >> 48) & 0xff : 0;
		SCpnt->cmnd[14] = sizeof(block) > 4 ? (unsigned char) (block >> 40) & 0xff : 0;
		SCpnt->cmnd[15] = sizeof(block) > 4 ? (unsigned char) (block >> 32) & 0xff : 0;
		SCpnt->cmnd[16] = (unsigned char) (block >> 24) & 0xff;
		SCpnt->cmnd[17] = (unsigned char) (block >> 16) & 0xff;
		SCpnt->cmnd[18] = (unsigned char) (block >> 8) & 0xff;
		SCpnt->cmnd[19] = (unsigned char) block & 0xff;

		 
		SCpnt->cmnd[20] = (unsigned char) (block >> 24) & 0xff;
		SCpnt->cmnd[21] = (unsigned char) (block >> 16) & 0xff;
		SCpnt->cmnd[22] = (unsigned char) (block >> 8) & 0xff;
		SCpnt->cmnd[23] = (unsigned char) block & 0xff;

		 
		SCpnt->cmnd[28] = (unsigned char) (this_count >> 24) & 0xff;
		SCpnt->cmnd[29] = (unsigned char) (this_count >> 16) & 0xff;
		SCpnt->cmnd[30] = (unsigned char) (this_count >> 8) & 0xff;
		SCpnt->cmnd[31] = (unsigned char) this_count & 0xff;
	} else if (block > 0xffffffff) {
		SCpnt->cmnd[0] += READ_16 - READ_6;
		SCpnt->cmnd[1] = protect | ((rq->cmd_flags & REQ_FUA) ? 0x8 : 0);
		SCpnt->cmnd[2] = sizeof(block) > 4 ? (unsigned char) (block >> 56) & 0xff : 0;
		SCpnt->cmnd[3] = sizeof(block) > 4 ? (unsigned char) (block >> 48) & 0xff : 0;
		SCpnt->cmnd[4] = sizeof(block) > 4 ? (unsigned char) (block >> 40) & 0xff : 0;
		SCpnt->cmnd[5] = sizeof(block) > 4 ? (unsigned char) (block >> 32) & 0xff : 0;
		SCpnt->cmnd[6] = (unsigned char) (block >> 24) & 0xff;
		SCpnt->cmnd[7] = (unsigned char) (block >> 16) & 0xff;
		SCpnt->cmnd[8] = (unsigned char) (block >> 8) & 0xff;
		SCpnt->cmnd[9] = (unsigned char) block & 0xff;
		SCpnt->cmnd[10] = (unsigned char) (this_count >> 24) & 0xff;
		SCpnt->cmnd[11] = (unsigned char) (this_count >> 16) & 0xff;
		SCpnt->cmnd[12] = (unsigned char) (this_count >> 8) & 0xff;
		SCpnt->cmnd[13] = (unsigned char) this_count & 0xff;
		SCpnt->cmnd[14] = SCpnt->cmnd[15] = 0;
	} else if ((this_count > 0xff) || (block > 0x1fffff) ||
		   scsi_device_protection(SCpnt->device) ||
		   SCpnt->device->use_10_for_rw) {
		if (this_count > 0xffff)
			this_count = 0xffff;

		SCpnt->cmnd[0] += READ_10 - READ_6;
		SCpnt->cmnd[1] = protect | ((rq->cmd_flags & REQ_FUA) ? 0x8 : 0);
		SCpnt->cmnd[2] = (unsigned char) (block >> 24) & 0xff;
		SCpnt->cmnd[3] = (unsigned char) (block >> 16) & 0xff;
		SCpnt->cmnd[4] = (unsigned char) (block >> 8) & 0xff;
		SCpnt->cmnd[5] = (unsigned char) block & 0xff;
		SCpnt->cmnd[6] = SCpnt->cmnd[9] = 0;
		SCpnt->cmnd[7] = (unsigned char) (this_count >> 8) & 0xff;
		SCpnt->cmnd[8] = (unsigned char) this_count & 0xff;
	} else {
		if (unlikely(rq->cmd_flags & REQ_FUA)) {
			 
			scmd_printk(KERN_ERR, SCpnt,
				    "FUA write on READ/WRITE(6) drive\n");
			goto out;
		}

		SCpnt->cmnd[1] |= (unsigned char) ((block >> 16) & 0x1f);
		SCpnt->cmnd[2] = (unsigned char) ((block >> 8) & 0xff);
		SCpnt->cmnd[3] = (unsigned char) block & 0xff;
		SCpnt->cmnd[4] = (unsigned char) this_count;
		SCpnt->cmnd[5] = 0;
	}
	SCpnt->sdb.length = this_count * sdp->sector_size;

	 
	if (host_dif || scsi_prot_sg_count(SCpnt))
		sd_prot_op(SCpnt, host_dif);

	 
	SCpnt->transfersize = sdp->sector_size;
	SCpnt->underflow = this_count << 9;
	SCpnt->allowed = SD_MAX_RETRIES;

	 
	ret = BLKPREP_OK;
 out:
	return scsi_prep_return(q, rq, ret);
}