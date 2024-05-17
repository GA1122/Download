void usb_hub_cleanup(void)
{
	destroy_workqueue(hub_wq);

	 
	usb_deregister(&hub_driver);
}  
