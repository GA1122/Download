int udf_put_filename(struct super_block *sb, const uint8_t *sname,
		     uint8_t *dname, int flen)
{
	struct ustr unifilename;
	int namelen;

	if (!udf_char_to_ustr(&unifilename, sname, flen))
		return 0;

	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8)) {
		namelen = udf_UTF8toCS0(dname, &unifilename, UDF_NAME_LEN);
		if (!namelen)
			return 0;
	} else if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP)) {
		namelen = udf_NLStoCS0(UDF_SB(sb)->s_nls_map, dname,
					&unifilename, UDF_NAME_LEN);
		if (!namelen)
			return 0;
	} else
		return 0;

	return namelen;
}