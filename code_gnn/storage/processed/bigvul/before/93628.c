nvmet_fc_rcv_fcp_abort(struct nvmet_fc_target_port *target_port,
			struct nvmefc_tgt_fcp_req *fcpreq)
{
	struct nvmet_fc_fcp_iod *fod = fcpreq->nvmet_fc_private;
	struct nvmet_fc_tgt_queue *queue;
	unsigned long flags;

	if (!fod || fod->fcpreq != fcpreq)
		 
		return;

	queue = fod->queue;

	spin_lock_irqsave(&queue->qlock, flags);
	if (fod->active) {
		 
		spin_lock(&fod->flock);
		fod->abort = true;
		fod->aborted = true;
		spin_unlock(&fod->flock);
	}
	spin_unlock_irqrestore(&queue->qlock, flags);
}