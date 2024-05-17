static void set_bootcount_addr(void)
{
	uchar buf[32];
	unsigned int bootcountaddr;
	bootcountaddr = gd->ram_size - BOOTCOUNT_ADDR;
	sprintf((char *)buf, "0x%x", bootcountaddr);
	env_set("bootcountaddr", (char *)buf);
}
