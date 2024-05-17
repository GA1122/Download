static void propagate_liveness(const struct bpf_verifier_state *state,
			       struct bpf_verifier_state *parent)
{
	while (do_propagate_liveness(state, parent)) {
		 
		state = parent;
		parent = state->parent;
	}
}
