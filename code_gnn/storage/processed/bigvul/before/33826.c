static int cipso_v4_genopt(unsigned char *buf, u32 buf_len,
			   const struct cipso_v4_doi *doi_def,
			   const struct netlbl_lsm_secattr *secattr)
{
	int ret_val;
	u32 iter;

	if (buf_len <= CIPSO_V4_HDR_LEN)
		return -ENOSPC;

	 
	iter = 0;
	do {
		memset(buf, 0, buf_len);
		switch (doi_def->tags[iter]) {
		case CIPSO_V4_TAG_RBITMAP:
			ret_val = cipso_v4_gentag_rbm(doi_def,
						   secattr,
						   &buf[CIPSO_V4_HDR_LEN],
						   buf_len - CIPSO_V4_HDR_LEN);
			break;
		case CIPSO_V4_TAG_ENUM:
			ret_val = cipso_v4_gentag_enum(doi_def,
						   secattr,
						   &buf[CIPSO_V4_HDR_LEN],
						   buf_len - CIPSO_V4_HDR_LEN);
			break;
		case CIPSO_V4_TAG_RANGE:
			ret_val = cipso_v4_gentag_rng(doi_def,
						   secattr,
						   &buf[CIPSO_V4_HDR_LEN],
						   buf_len - CIPSO_V4_HDR_LEN);
			break;
		case CIPSO_V4_TAG_LOCAL:
			ret_val = cipso_v4_gentag_loc(doi_def,
						   secattr,
						   &buf[CIPSO_V4_HDR_LEN],
						   buf_len - CIPSO_V4_HDR_LEN);
			break;
		default:
			return -EPERM;
		}

		iter++;
	} while (ret_val < 0 &&
		 iter < CIPSO_V4_TAG_MAXCNT &&
		 doi_def->tags[iter] != CIPSO_V4_TAG_INVALID);
	if (ret_val < 0)
		return ret_val;
	cipso_v4_gentag_hdr(doi_def, buf, ret_val);
	return CIPSO_V4_HDR_LEN + ret_val;
}