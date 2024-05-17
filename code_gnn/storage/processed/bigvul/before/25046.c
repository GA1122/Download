void rand_initialize_irq(int irq)
{
	struct timer_rand_state *state;

	state = get_timer_rand_state(irq);

	if (state)
		return;

	 
	state = kzalloc(sizeof(struct timer_rand_state), GFP_KERNEL);
	if (state)
		set_timer_rand_state(irq, state);
}
