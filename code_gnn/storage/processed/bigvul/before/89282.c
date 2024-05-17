void qrio_gpio_direction_output(u8 port_off, u8 gpio_nr, bool value)
{
	u32 direct, mask;

	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	mask = 1U << gpio_nr;

	direct = in_be32(qrio_base + port_off + DIRECT_OFF);
	direct |= mask;
	out_be32(qrio_base + port_off + DIRECT_OFF, direct);

	qrio_set_gpio(port_off, gpio_nr, value);
}
