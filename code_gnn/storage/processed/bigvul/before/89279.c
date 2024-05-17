void qrio_enable_app_buffer(void)
{
	u8 ctrll;
	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	 
	ctrll = in_8(qrio_base + CTRLL_OFF);
	ctrll |= (CTRLL_WRB_BUFENA);
	out_8(qrio_base + CTRLL_OFF, ctrll);
}
