void qrio_set_gpio(u8 port_off, u8 gpio_nr, bool value)
{
	u32 gprt, mask;

	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	mask = 1U << gpio_nr;

	gprt = in_be32(qrio_base + port_off + GPRT_OFF);
	if (value)
		gprt |= mask;
	else
		gprt &= ~mask;

	out_be32(qrio_base + port_off + GPRT_OFF, gprt);
}
