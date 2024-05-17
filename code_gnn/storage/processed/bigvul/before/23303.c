static int decode_getdeviceinfo(struct xdr_stream *xdr,
				struct pnfs_device *pdev)
{
	__be32 *p;
	uint32_t len, type;
	int status;

	status = decode_op_hdr(xdr, OP_GETDEVICEINFO);
	if (status) {
		if (status == -ETOOSMALL) {
			p = xdr_inline_decode(xdr, 4);
			if (unlikely(!p))
				goto out_overflow;
			pdev->mincount = be32_to_cpup(p);
			dprintk("%s: Min count too small. mincnt = %u\n",
				__func__, pdev->mincount);
		}
		return status;
	}

	p = xdr_inline_decode(xdr, 8);
	if (unlikely(!p))
		goto out_overflow;
	type = be32_to_cpup(p++);
	if (type != pdev->layout_type) {
		dprintk("%s: layout mismatch req: %u pdev: %u\n",
			__func__, pdev->layout_type, type);
		return -EINVAL;
	}
	 
	pdev->mincount = be32_to_cpup(p);
	xdr_read_pages(xdr, pdev->mincount);  

	 
	p = xdr_inline_decode(xdr, 4);
	if (unlikely(!p))
		goto out_overflow;
	len = be32_to_cpup(p);
	if (len) {
		uint32_t i;

		p = xdr_inline_decode(xdr, 4 * len);
		if (unlikely(!p))
			goto out_overflow;
		for (i = 0; i < len; i++, p++) {
			if (be32_to_cpup(p)) {
				dprintk("%s: notifications not supported\n",
					__func__);
				return -EIO;
			}
		}
	}
	return 0;
out_overflow:
	print_overflow_msg(__func__, xdr);
	return -EIO;
}
