static int opl3_detect(int ioaddr)
{
	 

	unsigned char stat1, signature;
	int i;

	if (devc != NULL)
	{
		printk(KERN_ERR "opl3: Only one OPL3 supported.\n");
		return 0;
	}

	devc = kzalloc(sizeof(*devc), GFP_KERNEL);

	if (devc == NULL)
	{
		printk(KERN_ERR "opl3: Can't allocate memory for the device control "
			"structure \n ");
		return 0;
	}

	strcpy(devc->fm_info.name, "OPL2");

	if (!request_region(ioaddr, 4, devc->fm_info.name)) {
		printk(KERN_WARNING "opl3: I/O port 0x%x already in use\n", ioaddr);
		goto cleanup_devc;
	}

	devc->base = ioaddr;

	 
	opl3_command(ioaddr, TIMER_CONTROL_REGISTER, TIMER1_MASK | TIMER2_MASK);

	 
	opl3_command(ioaddr, TIMER_CONTROL_REGISTER, IRQ_RESET);

	signature = stat1 = inb(ioaddr);	 

	if (signature != 0x00 && signature != 0x06 && signature != 0x02 &&
		signature != 0x0f)
	{
		MDB(printk(KERN_INFO "OPL3 not detected %x\n", signature));
		goto cleanup_region;
	}

	if (signature == 0x06)		 
	{
		detected_model = 2;
	}
	else if (signature == 0x00 || signature == 0x0f)	 
	{
		unsigned char tmp;

		detected_model = 3;

		 

		opl3_command(ioaddr + 2, OPL3_MODE_REGISTER, 0x00);
		opl3_command(ioaddr + 2, OPL3_MODE_REGISTER, OPL3_ENABLE | OPL4_ENABLE);

		if ((tmp = inb(ioaddr)) == 0x02)	 
		{
			detected_model = 4;
		}

		if (request_region(ioaddr - 8, 2, "OPL4"))	 
		{
			int tmp;

			outb((0x02), ioaddr - 8);	 
			udelay(10);
			tmp = inb(ioaddr - 7);		 
			udelay(10);

			if (tmp == 0x20)	 
			{
				detected_model = 4;
				outb((0xF8), ioaddr - 8);	 
				udelay(10);
				outb((0x1B), ioaddr - 7);	 
				udelay(10);
			}
			else
			{  
				release_region(ioaddr - 8, 2);
				detected_model = 3;
			}
		}
		opl3_command(ioaddr + 2, OPL3_MODE_REGISTER, 0);
	}
	for (i = 0; i < 9; i++)
		opl3_command(ioaddr, KEYON_BLOCK + i, 0);	 

	opl3_command(ioaddr, TEST_REGISTER, ENABLE_WAVE_SELECT);
	opl3_command(ioaddr, PERCOSSION_REGISTER, 0x00);	 
	return 1;
cleanup_region:
	release_region(ioaddr, 4);
cleanup_devc:
	kfree(devc);
	devc = NULL;
	return 0;
}
