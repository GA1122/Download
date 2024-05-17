void blk_pm_runtime_init(struct request_queue *q, struct device *dev)
{
	 
	if (q->mq_ops)
		return;

	q->dev = dev;
	q->rpm_status = RPM_ACTIVE;
	pm_runtime_set_autosuspend_delay(q->dev, -1);
	pm_runtime_use_autosuspend(q->dev);
}
