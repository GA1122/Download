void qrio_set_opendrain_gpio(u8 port_off, u8 gpio_nr, u8 val)
{
	u32 direct, mask;

	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	mask = 1U << gpio_nr;

	direct = in_be32(qrio_base + port_off + DIRECT_OFF);
	if (val == 0)
		 
		direct |= mask;
	else
		 
		direct &= ~mask;

	out_be32(qrio_base + port_off + DIRECT_OFF, direct);
}
