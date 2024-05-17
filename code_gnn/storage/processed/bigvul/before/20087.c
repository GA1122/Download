mii_write (struct net_device *dev, int phy_addr, int reg_num, u16 data)
{
	u32 cmd;

	 
	mii_send_bits (dev, 0xffffffff, 32);
	 
	 
	cmd = (0x5002 << 16) | (phy_addr << 23) | (reg_num << 18) | data;
	mii_send_bits (dev, cmd, 32);
	 
	mii_getbit (dev);
	return 0;
}
