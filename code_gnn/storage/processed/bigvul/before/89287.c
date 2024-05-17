 void qrio_uprstreq(u8 mode)
{
	u32 rstcfg;
	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	rstcfg = in_8(qrio_base + RSTCFG_OFF);

	if (mode & UPREQ_CORE_RST)
		rstcfg |= UPREQ_CORE_RST;
	else
		rstcfg &= ~UPREQ_CORE_RST;
 
 	out_8(qrio_base + RSTCFG_OFF, rstcfg);
 }
