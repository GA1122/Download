static bool is_flow_key_reg(struct bpf_verifier_env *env, int regno)
{
	const struct bpf_reg_state *reg = reg_state(env, regno);

	 
	return reg->type == PTR_TO_FLOW_KEYS;
}
