static void _tw32_flush(struct tg3 *tp, u32 off, u32 val, u32 usec_wait)
{
	if (tg3_flag(tp, PCIX_TARGET_HWBUG) || tg3_flag(tp, ICH_WORKAROUND))
		 
		tp->write32(tp, off, val);
	else {
		 
		tg3_write32(tp, off, val);
		if (usec_wait)
			udelay(usec_wait);
		tp->read32(tp, off);
	}
	 
	if (usec_wait)
		udelay(usec_wait);
}
