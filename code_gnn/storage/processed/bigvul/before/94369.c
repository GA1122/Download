static int sg_io(struct request_queue *q, struct gendisk *bd_disk,
		struct sg_io_hdr *hdr, fmode_t mode)
{
	unsigned long start_time;
	int writing = 0, ret = 0;
	struct request *rq;
	char sense[SCSI_SENSE_BUFFERSIZE];
	struct bio *bio;

	if (hdr->interface_id != 'S')
		return -EINVAL;
	if (hdr->cmd_len > BLK_MAX_CDB)
		return -EINVAL;

	if (hdr->dxfer_len > (queue_max_hw_sectors(q) << 9))
		return -EIO;

	if (hdr->dxfer_len)
		switch (hdr->dxfer_direction) {
		default:
			return -EINVAL;
		case SG_DXFER_TO_DEV:
			writing = 1;
			break;
		case SG_DXFER_TO_FROM_DEV:
		case SG_DXFER_FROM_DEV:
			break;
		}

	rq = blk_get_request(q, writing ? WRITE : READ, GFP_KERNEL);
	if (!rq)
		return -ENOMEM;

	if (blk_fill_sghdr_rq(q, rq, hdr, mode)) {
		blk_put_request(rq);
		return -EFAULT;
	}

	if (hdr->iovec_count) {
		const int size = sizeof(struct sg_iovec) * hdr->iovec_count;
		size_t iov_data_len;
		struct sg_iovec *sg_iov;
		struct iovec *iov;
		int i;

		sg_iov = kmalloc(size, GFP_KERNEL);
		if (!sg_iov) {
			ret = -ENOMEM;
			goto out;
		}

		if (copy_from_user(sg_iov, hdr->dxferp, size)) {
			kfree(sg_iov);
			ret = -EFAULT;
			goto out;
		}

		 
		iov = (struct iovec *) sg_iov;
		iov_data_len = 0;
		for (i = 0; i < hdr->iovec_count; i++) {
			if (iov_data_len + iov[i].iov_len < iov_data_len) {
				kfree(sg_iov);
				ret = -EINVAL;
				goto out;
			}
			iov_data_len += iov[i].iov_len;
		}

		 
		if (hdr->dxfer_len < iov_data_len) {
			hdr->iovec_count = iov_shorten(iov,
						       hdr->iovec_count,
						       hdr->dxfer_len);
			iov_data_len = hdr->dxfer_len;
		}

		ret = blk_rq_map_user_iov(q, rq, NULL, sg_iov, hdr->iovec_count,
					  iov_data_len, GFP_KERNEL);
		kfree(sg_iov);
	} else if (hdr->dxfer_len)
		ret = blk_rq_map_user(q, rq, NULL, hdr->dxferp, hdr->dxfer_len,
				      GFP_KERNEL);

	if (ret)
		goto out;

	bio = rq->bio;
	memset(sense, 0, sizeof(sense));
	rq->sense = sense;
	rq->sense_len = 0;
	rq->retries = 0;

	start_time = jiffies;

	 
	blk_execute_rq(q, bd_disk, rq, 0);

	hdr->duration = jiffies_to_msecs(jiffies - start_time);

	return blk_complete_sghdr_rq(rq, hdr, bio);
out:
	blk_put_request(rq);
	return ret;
}