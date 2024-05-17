static int blk_complete_sghdr_rq(struct request *rq, struct sg_io_hdr *hdr,
				 struct bio *bio)
{
	int r, ret = 0;

	 
	hdr->status = rq->errors & 0xff;
	hdr->masked_status = status_byte(rq->errors);
	hdr->msg_status = msg_byte(rq->errors);
	hdr->host_status = host_byte(rq->errors);
	hdr->driver_status = driver_byte(rq->errors);
	hdr->info = 0;
	if (hdr->masked_status || hdr->host_status || hdr->driver_status)
		hdr->info |= SG_INFO_CHECK;
	hdr->resid = rq->resid_len;
	hdr->sb_len_wr = 0;

	if (rq->sense_len && hdr->sbp) {
		int len = min((unsigned int) hdr->mx_sb_len, rq->sense_len);

		if (!copy_to_user(hdr->sbp, rq->sense, len))
			hdr->sb_len_wr = len;
		else
			ret = -EFAULT;
	}

	r = blk_rq_unmap_user(bio);
	if (!ret)
		ret = r;
	blk_put_request(rq);

	return ret;
}