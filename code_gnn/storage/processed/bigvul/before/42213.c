void vhost_dev_reset_owner(struct vhost_dev *dev, struct vhost_memory *memory)
{
	int i;

	vhost_dev_cleanup(dev, true);

	 
	memory->nregions = 0;
	dev->memory = memory;
	 
	for (i = 0; i < dev->nvqs; ++i)
		dev->vqs[i]->memory = memory;
}
