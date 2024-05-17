void qrio_wdmask(u8 bit, bool wden)
{
	u16 wdmask;
	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	wdmask = in_be16(qrio_base + WDMASK_OFF);

	if (wden)
		wdmask |= (1 << bit);
	else
		wdmask &= ~(1 << bit);

	out_be16(qrio_base + WDMASK_OFF, wdmask);
}
