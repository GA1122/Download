LogLuv24toXYZ(uint32 p, float XYZ[3])
{
	int	Ce;
	double	L, u, v, s, x, y;
					 
	L = LogL10toY(p>>14 & 0x3ff);
	if (L <= 0.) {
		XYZ[0] = XYZ[1] = XYZ[2] = 0.;
		return;
	}
					 
	Ce = p & 0x3fff;
	if (uv_decode(&u, &v, Ce) < 0) {
		u = U_NEU; v = V_NEU;
	}
	s = 1./(6.*u - 16.*v + 12.);
	x = 9.*u * s;
	y = 4.*v * s;
					 
	XYZ[0] = (float)(x/y * L);
	XYZ[1] = (float)L;
	XYZ[2] = (float)((1.-x-y)/y * L);
}
