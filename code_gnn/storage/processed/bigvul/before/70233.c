LogLuv24fromXYZ(float XYZ[3], int em)
{
	int	Le, Ce;
	double	u, v, s;
					 
	Le = LogL10fromY(XYZ[1], em);
					 
	s = XYZ[0] + 15.*XYZ[1] + 3.*XYZ[2];
	if (!Le || s <= 0.) {
		u = U_NEU;
		v = V_NEU;
	} else {
		u = 4.*XYZ[0] / s;
		v = 9.*XYZ[1] / s;
	}
	Ce = uv_encode(u, v, em);
	if (Ce < 0)			 
		Ce = uv_encode(U_NEU, V_NEU, SGILOGENCODE_NODITHER);
					 
	return (Le << 14 | Ce);
}
