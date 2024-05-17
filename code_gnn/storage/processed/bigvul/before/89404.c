static void __attribute__((no_instrument_function)) trace_swap_gd(void)
{
	volatile void *temp_gd = trace_gd;

	trace_gd = gd;
	gd = temp_gd;
}
