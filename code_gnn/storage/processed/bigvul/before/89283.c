void qrio_prst(u8 bit, bool en, bool wden)
{
	u16 prst;
	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	qrio_wdmask(bit, wden);

	prst = in_be16(qrio_base + PRST_OFF);

	if (en)
		prst &= ~(1 << bit);
	else
		prst |= (1 << bit);

	out_be16(qrio_base + PRST_OFF, prst);
}
