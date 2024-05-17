int rds_rdma_extra_size(struct rds_rdma_args *args)
{
	struct rds_iovec vec;
	struct rds_iovec __user *local_vec;
	int tot_pages = 0;
	unsigned int nr_pages;
	unsigned int i;

	local_vec = (struct rds_iovec __user *)(unsigned long) args->local_vec_addr;

	 
	for (i = 0; i < args->nr_local; i++) {
		if (copy_from_user(&vec, &local_vec[i],
				   sizeof(struct rds_iovec)))
			return -EFAULT;

		nr_pages = rds_pages_in_vec(&vec);
		if (nr_pages == 0)
			return -EINVAL;

		tot_pages += nr_pages;

		 
		if (tot_pages < 0)
			return -EINVAL;
	}

	return tot_pages * sizeof(struct scatterlist);
}
