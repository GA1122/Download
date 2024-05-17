cleanup_bmc_device(struct kref *ref)
{
	struct bmc_device *bmc = container_of(ref, struct bmc_device, usecount);

	 
	schedule_work(&bmc->remove_work);
}
