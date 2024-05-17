static void add_timer_randomness(struct timer_rand_state *state, unsigned num)
{
	struct {
		cycles_t cycles;
		long jiffies;
		unsigned num;
	} sample;
	long delta, delta2, delta3;

	preempt_disable();
	 
	if (input_pool.entropy_count > trickle_thresh &&
	    ((__this_cpu_inc_return(trickle_count) - 1) & 0xfff))
		goto out;

	sample.jiffies = jiffies;
	sample.cycles = get_cycles();
	sample.num = num;
	mix_pool_bytes(&input_pool, &sample, sizeof(sample));

	 

	if (!state->dont_count_entropy) {
		delta = sample.jiffies - state->last_time;
		state->last_time = sample.jiffies;

		delta2 = delta - state->last_delta;
		state->last_delta = delta;

		delta3 = delta2 - state->last_delta2;
		state->last_delta2 = delta2;

		if (delta < 0)
			delta = -delta;
		if (delta2 < 0)
			delta2 = -delta2;
		if (delta3 < 0)
			delta3 = -delta3;
		if (delta > delta2)
			delta = delta2;
		if (delta > delta3)
			delta = delta3;

		 
		credit_entropy_bits(&input_pool,
				    min_t(int, fls(delta>>1), 11));
	}
out:
	preempt_enable();
}
