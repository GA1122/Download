void svc_rdma_unmap_dma(struct svc_rdma_op_ctxt *ctxt)
{
	struct svcxprt_rdma *xprt = ctxt->xprt;
	struct ib_device *device = xprt->sc_cm_id->device;
	u32 lkey = xprt->sc_pd->local_dma_lkey;
	unsigned int i;

	for (i = 0; i < ctxt->mapped_sges; i++) {
		 
		if (ctxt->sge[i].lkey == lkey)
			ib_dma_unmap_page(device,
					    ctxt->sge[i].addr,
					    ctxt->sge[i].length,
					    ctxt->direction);
	}
	ctxt->mapped_sges = 0;
}
