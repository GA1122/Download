static int validate_scan_freqs(struct nlattr *freqs)
{
	struct nlattr *attr1, *attr2;
	int n_channels = 0, tmp1, tmp2;

	nla_for_each_nested(attr1, freqs, tmp1) {
		n_channels++;
		 
		nla_for_each_nested(attr2, freqs, tmp2)
			if (attr1 != attr2 &&
			    nla_get_u32(attr1) == nla_get_u32(attr2))
				return 0;
	}

	return n_channels;
}
