void rand_initialize_disk(struct gendisk *disk)
{
	struct timer_rand_state *state;

	 
	state = kzalloc(sizeof(struct timer_rand_state), GFP_KERNEL);
	if (state)
		disk->random = state;
}
