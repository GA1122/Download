static iw_float32 iw_get_float32(const iw_byte *m)
{
	int k;
	union su_union {
		iw_byte c[4];
		iw_float32 f;
	} volatile su;

	for(k=0;k<4;k++) {
		su.c[k] = m[k];
	}
	return su.f;
}