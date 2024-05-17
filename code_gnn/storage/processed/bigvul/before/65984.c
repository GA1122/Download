static struct svc_rdma_fastreg_mr *rdma_alloc_frmr(struct svcxprt_rdma *xprt)
{
	struct ib_mr *mr;
	struct scatterlist *sg;
	struct svc_rdma_fastreg_mr *frmr;
	u32 num_sg;

	frmr = kmalloc(sizeof(*frmr), GFP_KERNEL);
	if (!frmr)
		goto err;

	num_sg = min_t(u32, RPCSVC_MAXPAGES, xprt->sc_frmr_pg_list_len);
	mr = ib_alloc_mr(xprt->sc_pd, IB_MR_TYPE_MEM_REG, num_sg);
	if (IS_ERR(mr))
		goto err_free_frmr;

	sg = kcalloc(RPCSVC_MAXPAGES, sizeof(*sg), GFP_KERNEL);
	if (!sg)
		goto err_free_mr;

	sg_init_table(sg, RPCSVC_MAXPAGES);

	frmr->mr = mr;
	frmr->sg = sg;
	INIT_LIST_HEAD(&frmr->frmr_list);
	return frmr;

 err_free_mr:
	ib_dereg_mr(mr);
 err_free_frmr:
	kfree(frmr);
 err:
	return ERR_PTR(-ENOMEM);
}
