static int iw_parse_number_internal(const char *s,
		   double *presult, int *pcharsread)
{
	int len;
	int slash_pos = -1;

	*presult = 0.0;
	*pcharsread = 0;

	len = iw_get_number_len(s,&slash_pos);
	if(len<1) return 0;
	*pcharsread = len;

	if(slash_pos>=0) {
		double numer, denom;
		numer = atof(s);
		denom = atof(s+slash_pos+1);
		if(denom==0.0)
			*presult = 0.0;
		else
			*presult = numer/denom;
	}
	else {
		*presult = atof(s);
	}
	return 1;
}