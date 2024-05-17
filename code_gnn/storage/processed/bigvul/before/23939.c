static void add_airo_dev(struct airo_info *ai)
{
	 
	if (!ai->pci)
		list_add_tail(&ai->dev_list, &airo_devices);
}
