void __init early_platform_cleanup(void)
{
	struct platform_device *pd, *pd2;

	 
	list_for_each_entry_safe(pd, pd2, &early_platform_device_list,
				 dev.devres_head) {
		list_del(&pd->dev.devres_head);
		memset(&pd->dev.devres_head, 0, sizeof(pd->dev.devres_head));
	}
}