static int srpt_map_sg_to_ib_sge(struct srpt_rdma_ch *ch,
				 struct srpt_send_ioctx *ioctx)
{
	struct ib_device *dev = ch->sport->sdev->device;
	struct se_cmd *cmd;
	struct scatterlist *sg, *sg_orig;
	int sg_cnt;
	enum dma_data_direction dir;
	struct ib_rdma_wr *riu;
	struct srp_direct_buf *db;
	dma_addr_t dma_addr;
	struct ib_sge *sge;
	u64 raddr;
	u32 rsize;
	u32 tsize;
	u32 dma_len;
	int count, nrdma;
	int i, j, k;

	BUG_ON(!ch);
	BUG_ON(!ioctx);
	cmd = &ioctx->cmd;
	dir = cmd->data_direction;
	BUG_ON(dir == DMA_NONE);

	ioctx->sg = sg = sg_orig = cmd->t_data_sg;
	ioctx->sg_cnt = sg_cnt = cmd->t_data_nents;

	count = ib_dma_map_sg(ch->sport->sdev->device, sg, sg_cnt,
			      opposite_dma_dir(dir));
	if (unlikely(!count))
		return -EAGAIN;

	ioctx->mapped_sg_count = count;

	if (ioctx->rdma_wrs && ioctx->n_rdma_wrs)
		nrdma = ioctx->n_rdma_wrs;
	else {
		nrdma = (count + SRPT_DEF_SG_PER_WQE - 1) / SRPT_DEF_SG_PER_WQE
			+ ioctx->n_rbuf;

		ioctx->rdma_wrs = kcalloc(nrdma, sizeof(*ioctx->rdma_wrs),
				GFP_KERNEL);
		if (!ioctx->rdma_wrs)
			goto free_mem;

		ioctx->n_rdma_wrs = nrdma;
	}

	db = ioctx->rbufs;
	tsize = cmd->data_length;
	dma_len = ib_sg_dma_len(dev, &sg[0]);
	riu = ioctx->rdma_wrs;

	 
	for (i = 0, j = 0;
	     j < count && i < ioctx->n_rbuf && tsize > 0; ++i, ++riu, ++db) {
		rsize = be32_to_cpu(db->len);
		raddr = be64_to_cpu(db->va);
		riu->remote_addr = raddr;
		riu->rkey = be32_to_cpu(db->key);
		riu->wr.num_sge = 0;

		 
		while (rsize > 0 && tsize > 0) {

			if (rsize >= dma_len) {
				tsize -= dma_len;
				rsize -= dma_len;
				raddr += dma_len;

				if (tsize > 0) {
					++j;
					if (j < count) {
						sg = sg_next(sg);
						dma_len = ib_sg_dma_len(
								dev, sg);
					}
				}
			} else {
				tsize -= rsize;
				dma_len -= rsize;
				rsize = 0;
			}

			++riu->wr.num_sge;

			if (rsize > 0 &&
			    riu->wr.num_sge == SRPT_DEF_SG_PER_WQE) {
				++ioctx->n_rdma;
				riu->wr.sg_list = kmalloc_array(riu->wr.num_sge,
						sizeof(*riu->wr.sg_list),
						GFP_KERNEL);
				if (!riu->wr.sg_list)
					goto free_mem;

				++riu;
				riu->wr.num_sge = 0;
				riu->remote_addr = raddr;
				riu->rkey = be32_to_cpu(db->key);
			}
		}

		++ioctx->n_rdma;
		riu->wr.sg_list = kmalloc_array(riu->wr.num_sge,
					sizeof(*riu->wr.sg_list),
					GFP_KERNEL);
		if (!riu->wr.sg_list)
			goto free_mem;
	}

	db = ioctx->rbufs;
	tsize = cmd->data_length;
	riu = ioctx->rdma_wrs;
	sg = sg_orig;
	dma_len = ib_sg_dma_len(dev, &sg[0]);
	dma_addr = ib_sg_dma_address(dev, &sg[0]);

	 
	for (i = 0, j = 0;
	     j < count && i < ioctx->n_rbuf && tsize > 0; ++i, ++riu, ++db) {
		rsize = be32_to_cpu(db->len);
		sge = riu->wr.sg_list;
		k = 0;

		while (rsize > 0 && tsize > 0) {
			sge->addr = dma_addr;
			sge->lkey = ch->sport->sdev->pd->local_dma_lkey;

			if (rsize >= dma_len) {
				sge->length =
					(tsize < dma_len) ? tsize : dma_len;
				tsize -= dma_len;
				rsize -= dma_len;

				if (tsize > 0) {
					++j;
					if (j < count) {
						sg = sg_next(sg);
						dma_len = ib_sg_dma_len(
								dev, sg);
						dma_addr = ib_sg_dma_address(
								dev, sg);
					}
				}
			} else {
				sge->length = (tsize < rsize) ? tsize : rsize;
				tsize -= rsize;
				dma_len -= rsize;
				dma_addr += rsize;
				rsize = 0;
			}

			++k;
			if (k == riu->wr.num_sge && rsize > 0 && tsize > 0) {
				++riu;
				sge = riu->wr.sg_list;
				k = 0;
			} else if (rsize > 0 && tsize > 0)
				++sge;
		}
	}

	return 0;

free_mem:
	srpt_unmap_sg_to_ib_sge(ch, ioctx);

	return -ENOMEM;
}