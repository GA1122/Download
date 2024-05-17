static u64 check_and_compute_delta(u64 prev, u64 val)
{
	u64 delta = (val - prev) & 0xfffffffful;

	 
	if (prev > val && (prev - val) < 256)
		delta = 0;

	return delta;
}
