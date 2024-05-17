  static freelist_idx_t next_random_slot(union freelist_init_state *state)
  {
	return (state->list[state->pos++] + state->rand) % state->count;
// 	if (state->pos >= state->count)
// 		state->pos = 0;
// 	return state->list[state->pos++];
  }