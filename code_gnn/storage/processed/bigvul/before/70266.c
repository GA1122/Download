uv_encode(double u, double v, int em)	 
{
	register int	vi, ui;

	if (v < UV_VSTART)
		return oog_encode(u, v);
	vi = itrunc((v - UV_VSTART)*(1./UV_SQSIZ), em);
	if (vi >= UV_NVS)
		return oog_encode(u, v);
	if (u < uv_row[vi].ustart)
		return oog_encode(u, v);
	ui = itrunc((u - uv_row[vi].ustart)*(1./UV_SQSIZ), em);
	if (ui >= uv_row[vi].nus)
		return oog_encode(u, v);

	return (uv_row[vi].ncum + ui);
}
