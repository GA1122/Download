dispersion(const datapoint_t *dp)
{
	return dp->d_dispersion + FREQ_TOLERANCE * (G.cur_time - dp->d_recv_time);
}
