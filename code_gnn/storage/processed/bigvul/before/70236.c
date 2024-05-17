LogLuv32toXYZ(uint32 p, float XYZ[3])
{
	double	L, u, v, s, x, y;
					 
	L = LogL16toY((int)p >> 16);
	if (L <= 0.) {
		XYZ[0] = XYZ[1] = XYZ[2] = 0.;
		return;
	}
					 
	u = 1./UVSCALE * ((p>>8 & 0xff) + .5);
	v = 1./UVSCALE * ((p & 0xff) + .5);
	s = 1./(6.*u - 16.*v + 12.);
	x = 9.*u * s;
	y = 4.*v * s;
					 
	XYZ[0] = (float)(x/y * L);
	XYZ[1] = (float)L;
	XYZ[2] = (float)((1.-x-y)/y * L);
}