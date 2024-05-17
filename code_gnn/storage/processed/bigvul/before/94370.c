int sg_scsi_ioctl(struct request_queue *q, struct gendisk *disk, fmode_t mode,
		struct scsi_ioctl_command __user *sic)
{
	struct request *rq;
	int err;
	unsigned int in_len, out_len, bytes, opcode, cmdlen;
	char *buffer = NULL, sense[SCSI_SENSE_BUFFERSIZE];

	if (!sic)
		return -EINVAL;

	 
	if (get_user(in_len, &sic->inlen))
		return -EFAULT;
	if (get_user(out_len, &sic->outlen))
		return -EFAULT;
	if (in_len > PAGE_SIZE || out_len > PAGE_SIZE)
		return -EINVAL;
	if (get_user(opcode, sic->data))
		return -EFAULT;

	bytes = max(in_len, out_len);
	if (bytes) {
		buffer = kzalloc(bytes, q->bounce_gfp | GFP_USER| __GFP_NOWARN);
		if (!buffer)
			return -ENOMEM;

	}

	rq = blk_get_request(q, in_len ? WRITE : READ, __GFP_WAIT);

	cmdlen = COMMAND_SIZE(opcode);

	 
	err = -EFAULT;
	rq->cmd_len = cmdlen;
	if (copy_from_user(rq->cmd, sic->data, cmdlen))
		goto error;

	if (in_len && copy_from_user(buffer, sic->data + cmdlen, in_len))
		goto error;

	err = blk_verify_command(rq->cmd, mode & FMODE_WRITE);
	if (err)
		goto error;

	 
	rq->retries = 5;

	switch (opcode) {
	case SEND_DIAGNOSTIC:
	case FORMAT_UNIT:
		rq->timeout = FORMAT_UNIT_TIMEOUT;
		rq->retries = 1;
		break;
	case START_STOP:
		rq->timeout = START_STOP_TIMEOUT;
		break;
	case MOVE_MEDIUM:
		rq->timeout = MOVE_MEDIUM_TIMEOUT;
		break;
	case READ_ELEMENT_STATUS:
		rq->timeout = READ_ELEMENT_STATUS_TIMEOUT;
		break;
	case READ_DEFECT_DATA:
		rq->timeout = READ_DEFECT_DATA_TIMEOUT;
		rq->retries = 1;
		break;
	default:
		rq->timeout = BLK_DEFAULT_SG_TIMEOUT;
		break;
	}

	if (bytes && blk_rq_map_kern(q, rq, buffer, bytes, __GFP_WAIT)) {
		err = DRIVER_ERROR << 24;
		goto out;
	}

	memset(sense, 0, sizeof(sense));
	rq->sense = sense;
	rq->sense_len = 0;
	rq->cmd_type = REQ_TYPE_BLOCK_PC;

	blk_execute_rq(q, disk, rq, 0);

out:
	err = rq->errors & 0xff;	 
	if (err) {
		if (rq->sense_len && rq->sense) {
			bytes = (OMAX_SB_LEN > rq->sense_len) ?
				rq->sense_len : OMAX_SB_LEN;
			if (copy_to_user(sic->data, rq->sense, bytes))
				err = -EFAULT;
		}
	} else {
		if (copy_to_user(sic->data, buffer, out_len))
			err = -EFAULT;
	}
	
error:
	kfree(buffer);
	blk_put_request(rq);
	return err;
}