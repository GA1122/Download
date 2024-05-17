 nfs4_state_set_mode_locked(struct nfs4_state *state, mode_t mode)
// nfs4_state_set_mode_locked(struct nfs4_state *state, fmode_t fmode)
  {
	if (state->state == mode)
// 	if (state->state == fmode)
  		return;
  	 
	if ((mode & FMODE_WRITE) != (state->state & FMODE_WRITE)) {
		if (mode & FMODE_WRITE)
// 	if ((fmode & FMODE_WRITE) != (state->state & FMODE_WRITE)) {
// 		if (fmode & FMODE_WRITE)
  			list_move(&state->open_states, &state->owner->so_states);
  		else
  			list_move_tail(&state->open_states, &state->owner->so_states);
  	}
	state->state = mode;
// 	state->state = fmode;
  }