static int mark_reg_read(struct bpf_verifier_env *env,
			 const struct bpf_reg_state *state,
			 struct bpf_reg_state *parent)
{
	bool writes = parent == state->parent;  

	while (parent) {
		 
		if (writes && state->live & REG_LIVE_WRITTEN)
			break;
		if (parent->live & REG_LIVE_DONE) {
			verbose(env, "verifier BUG type %s var_off %lld off %d\n",
				reg_type_str[parent->type],
				parent->var_off.value, parent->off);
			return -EFAULT;
		}
		 
		parent->live |= REG_LIVE_READ;
		state = parent;
		parent = state->parent;
		writes = true;
	}
	return 0;
}
