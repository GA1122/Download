static int tg3_ptp_adjfreq(struct ptp_clock_info *ptp, s32 ppb)
{
	struct tg3 *tp = container_of(ptp, struct tg3, ptp_info);
	bool neg_adj = false;
	u32 correction = 0;

	if (ppb < 0) {
		neg_adj = true;
		ppb = -ppb;
	}

	 
	correction = div_u64((u64)ppb * (1 << 24), 1000000000ULL) &
		     TG3_EAV_REF_CLK_CORRECT_MASK;

	tg3_full_lock(tp, 0);

	if (correction)
		tw32(TG3_EAV_REF_CLK_CORRECT_CTL,
		     TG3_EAV_REF_CLK_CORRECT_EN |
		     (neg_adj ? TG3_EAV_REF_CLK_CORRECT_NEG : 0) | correction);
	else
		tw32(TG3_EAV_REF_CLK_CORRECT_CTL, 0);

	tg3_full_unlock(tp);

	return 0;
}