static int __init init_opl3 (void)
{
	printk(KERN_INFO "YM3812 and OPL-3 driver Copyright (C) by Hannu Savolainen, Rob Hooft 1993-1996\n");

	if (io != -1)	 
	{
		if (!opl3_detect(io))
		{
			return -ENODEV;
		}

		me = opl3_init(io, THIS_MODULE);
	}

	return 0;
}