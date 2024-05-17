void __init reserve_standard_io_resources(void)
{
	int i;

	 
	for (i = 0; i < ARRAY_SIZE(standard_io_resources); i++)
		request_resource(&ioport_resource, &standard_io_resources[i]);

}
