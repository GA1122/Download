void init_ikev1(void)
{
	 
	const struct state_microcode *t;

	for (t = &v1_state_microcode_table[elemsof(v1_state_microcode_table) - 1];;)
	{
		passert(STATE_IKE_FLOOR <= t->state &&
			t->state < STATE_IKE_ROOF);
		ike_microcode_index[t->state - STATE_IKE_FLOOR] = t;
		if (t == v1_state_microcode_table)
			break;
		t--;
		passert(t[0].state <= t[1].state);
	}
}
