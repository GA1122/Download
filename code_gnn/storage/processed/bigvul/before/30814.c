static int check_tp(const struct atm_trafprm *tp)
{
	 
	if (!tp->traffic_class || tp->traffic_class == ATM_ANYCLASS)
		return 0;
	if (tp->traffic_class != ATM_UBR && !tp->min_pcr && !tp->pcr &&
	    !tp->max_pcr)
		return -EINVAL;
	if (tp->min_pcr == ATM_MAX_PCR)
		return -EINVAL;
	if (tp->min_pcr && tp->max_pcr && tp->max_pcr != ATM_MAX_PCR &&
	    tp->min_pcr > tp->max_pcr)
		return -EINVAL;
	 
	return 0;
}
