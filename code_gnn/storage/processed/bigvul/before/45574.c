static inline int crypto_ccm_check_iv(const u8 *iv)
{
	 
	if (1 > iv[0] || iv[0] > 7)
		return -EINVAL;

	return 0;
}
