evutil_weakrand_range_(struct evutil_weakrand_state *state, ev_int32_t top)
{
	ev_int32_t divisor, result;

	 
	divisor = EVUTIL_WEAKRAND_MAX / top;
	do {
		result = evutil_weakrand_(state) / divisor;
	} while (result >= top);
	return result;
}
