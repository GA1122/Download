static void sas_ata_post_internal(struct ata_queued_cmd *qc)
{
	if (qc->flags & ATA_QCFLAG_FAILED)
		qc->err_mask |= AC_ERR_OTHER;

	if (qc->err_mask) {
		 
		struct sas_task *task = qc->lldd_task;

		qc->lldd_task = NULL;
		if (!task)
			return;
		task->uldd_task = NULL;
		sas_ata_internal_abort(task);
	}
}
