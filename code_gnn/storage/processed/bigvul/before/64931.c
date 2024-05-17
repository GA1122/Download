static void iw_put_float32(iw_byte *m, iw_float32 s)
{
	int k;
	union su_union {
		iw_byte c[4];
		iw_float32 f;
	} volatile su;

	su.f = s;

	for(k=0;k<4;k++) {
		m[k] = su.c[k];
	}
}