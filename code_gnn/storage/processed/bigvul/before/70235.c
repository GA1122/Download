LogLuv32fromXYZ(float XYZ[3], int em)
{
	unsigned int	Le, ue, ve;
	double	u, v, s;
					 
	Le = (unsigned int)LogL16fromY(XYZ[1], em);
					 
	s = XYZ[0] + 15.*XYZ[1] + 3.*XYZ[2];
	if (!Le || s <= 0.) {
		u = U_NEU;
		v = V_NEU;
	} else {
		u = 4.*XYZ[0] / s;
		v = 9.*XYZ[1] / s;
	}
	if (u <= 0.) ue = 0;
	else ue = itrunc(UVSCALE*u, em);
	if (ue > 255) ue = 255;
	if (v <= 0.) ve = 0;
	else ve = itrunc(UVSCALE*v, em);
	if (ve > 255) ve = 255;
					 
	return (Le << 16 | ue << 8 | ve);
}