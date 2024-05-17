sc_asn1_sig_value_rs_to_sequence(struct sc_context *ctx, unsigned char *in, size_t inlen,
		unsigned char **buf, size_t *buflen)
{
	struct sc_asn1_entry asn1_sig_value[C_ASN1_SIG_VALUE_SIZE];
	struct sc_asn1_entry asn1_sig_value_coefficients[C_ASN1_SIG_VALUE_COEFFICIENTS_SIZE];
	unsigned char *r = in, *s = in + inlen/2;
	size_t r_len = inlen/2, s_len = inlen/2;
	int rv;

	LOG_FUNC_CALLED(ctx);

	 
	while(r_len > 1 && *r == 0x00) {
		r++;
		r_len--;
	}
	while(s_len > 1 && *s == 0x00) {
		s++;
		s_len--;
	}

	sc_copy_asn1_entry(c_asn1_sig_value, asn1_sig_value);
	sc_format_asn1_entry(asn1_sig_value + 0, asn1_sig_value_coefficients, NULL, 1);

	sc_copy_asn1_entry(c_asn1_sig_value_coefficients, asn1_sig_value_coefficients);
	sc_format_asn1_entry(asn1_sig_value_coefficients + 0, r, &r_len, 1);
	sc_format_asn1_entry(asn1_sig_value_coefficients + 1, s, &s_len, 1);

	rv = sc_asn1_encode(ctx, asn1_sig_value, buf, buflen);
	LOG_TEST_RET(ctx, rv, "ASN.1 encoding ECDSA-SIg-Value failed");

	LOG_FUNC_RETURN(ctx, SC_SUCCESS);
}