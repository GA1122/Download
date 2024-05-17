int qrio_get_gpio(u8 port_off, u8 gpio_nr)
{
	u32 gprt;

	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	gprt = in_be32(qrio_base + port_off + GPRT_OFF);

	return (gprt >> gpio_nr) & 1U;
}
