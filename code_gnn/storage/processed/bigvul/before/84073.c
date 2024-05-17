int rds_cmsg_rdma_args(struct rds_sock *rs, struct rds_message *rm,
			  struct cmsghdr *cmsg)
{
	struct rds_rdma_args *args;
	struct rm_rdma_op *op = &rm->rdma;
	int nr_pages;
	unsigned int nr_bytes;
	struct page **pages = NULL;
	struct rds_iovec iovstack[UIO_FASTIOV], *iovs = iovstack;
	int iov_size;
	unsigned int i, j;
	int ret = 0;

	if (cmsg->cmsg_len < CMSG_LEN(sizeof(struct rds_rdma_args))
	    || rm->rdma.op_active)
		return -EINVAL;

	args = CMSG_DATA(cmsg);

	if (rs->rs_bound_addr == 0) {
		ret = -ENOTCONN;  
		goto out_ret;
	}

	if (args->nr_local > UIO_MAXIOV) {
		ret = -EMSGSIZE;
		goto out_ret;
	}

	 
	iov_size = args->nr_local * sizeof(struct rds_iovec);
	if (args->nr_local > UIO_FASTIOV) {
		iovs = sock_kmalloc(rds_rs_to_sk(rs), iov_size, GFP_KERNEL);
		if (!iovs) {
			ret = -ENOMEM;
			goto out_ret;
		}
	}

	if (copy_from_user(iovs, (struct rds_iovec __user *)(unsigned long) args->local_vec_addr, iov_size)) {
		ret = -EFAULT;
		goto out;
	}

	nr_pages = rds_rdma_pages(iovs, args->nr_local);
	if (nr_pages < 0) {
		ret = -EINVAL;
		goto out;
	}

	pages = kcalloc(nr_pages, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		ret = -ENOMEM;
		goto out;
	}

	op->op_write = !!(args->flags & RDS_RDMA_READWRITE);
	op->op_fence = !!(args->flags & RDS_RDMA_FENCE);
	op->op_notify = !!(args->flags & RDS_RDMA_NOTIFY_ME);
	op->op_silent = !!(args->flags & RDS_RDMA_SILENT);
	op->op_active = 1;
	op->op_recverr = rs->rs_recverr;
	WARN_ON(!nr_pages);
	op->op_sg = rds_message_alloc_sgs(rm, nr_pages);
	if (!op->op_sg) {
		ret = -ENOMEM;
		goto out;
	}

	if (op->op_notify || op->op_recverr) {
		 
		op->op_notifier = kmalloc(sizeof(struct rds_notifier), GFP_KERNEL);
		if (!op->op_notifier) {
			ret = -ENOMEM;
			goto out;
		}
		op->op_notifier->n_user_token = args->user_token;
		op->op_notifier->n_status = RDS_RDMA_SUCCESS;

		 
		if (rm->data.op_sg) {
			rm->rdma.op_notify = 0;
			rm->data.op_notify = !!(args->flags & RDS_RDMA_NOTIFY_ME);
		}
	}

	 
	op->op_rkey = rds_rdma_cookie_key(args->cookie);
	op->op_remote_addr = args->remote_vec.addr + rds_rdma_cookie_offset(args->cookie);

	nr_bytes = 0;

	rdsdebug("RDS: rdma prepare nr_local %llu rva %llx rkey %x\n",
	       (unsigned long long)args->nr_local,
	       (unsigned long long)args->remote_vec.addr,
	       op->op_rkey);

	for (i = 0; i < args->nr_local; i++) {
		struct rds_iovec *iov = &iovs[i];
		 
		unsigned int nr = rds_pages_in_vec(iov);

		rs->rs_user_addr = iov->addr;
		rs->rs_user_bytes = iov->bytes;

		 
		ret = rds_pin_pages(iov->addr, nr, pages, !op->op_write);
		if (ret < 0)
			goto out;
		else
			ret = 0;

		rdsdebug("RDS: nr_bytes %u nr %u iov->bytes %llu iov->addr %llx\n",
			 nr_bytes, nr, iov->bytes, iov->addr);

		nr_bytes += iov->bytes;

		for (j = 0; j < nr; j++) {
			unsigned int offset = iov->addr & ~PAGE_MASK;
			struct scatterlist *sg;

			sg = &op->op_sg[op->op_nents + j];
			sg_set_page(sg, pages[j],
					min_t(unsigned int, iov->bytes, PAGE_SIZE - offset),
					offset);

			rdsdebug("RDS: sg->offset %x sg->len %x iov->addr %llx iov->bytes %llu\n",
			       sg->offset, sg->length, iov->addr, iov->bytes);

			iov->addr += sg->length;
			iov->bytes -= sg->length;
		}

		op->op_nents += nr;
	}

	if (nr_bytes > args->remote_vec.bytes) {
		rdsdebug("RDS nr_bytes %u remote_bytes %u do not match\n",
				nr_bytes,
				(unsigned int) args->remote_vec.bytes);
		ret = -EINVAL;
		goto out;
	}
	op->op_bytes = nr_bytes;

out:
	if (iovs != iovstack)
		sock_kfree_s(rds_rs_to_sk(rs), iovs, iov_size);
	kfree(pages);
out_ret:
	if (ret)
		rds_rdma_free_op(op);
	else
		rds_stats_inc(s_send_rdma);

	return ret;
}
