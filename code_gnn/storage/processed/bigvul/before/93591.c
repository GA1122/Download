nvmet_fc_alloc_fcp_iod(struct nvmet_fc_tgt_queue *queue)
{
	struct nvmet_fc_fcp_iod *fod;

	lockdep_assert_held(&queue->qlock);

	fod = list_first_entry_or_null(&queue->fod_list,
					struct nvmet_fc_fcp_iod, fcp_list);
	if (fod) {
		list_del(&fod->fcp_list);
		fod->active = true;
		 
	}
	return fod;
}
