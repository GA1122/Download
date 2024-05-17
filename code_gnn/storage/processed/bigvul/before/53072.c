static void fixup_bpf_calls(struct bpf_prog *prog)
{
	const struct bpf_func_proto *fn;
	int i;

	for (i = 0; i < prog->len; i++) {
		struct bpf_insn *insn = &prog->insnsi[i];

		if (insn->code == (BPF_JMP | BPF_CALL)) {
			 
			BUG_ON(!prog->aux->ops->get_func_proto);

			if (insn->imm == BPF_FUNC_get_route_realm)
				prog->dst_needed = 1;
			if (insn->imm == BPF_FUNC_get_prandom_u32)
				bpf_user_rnd_init_once();
			if (insn->imm == BPF_FUNC_tail_call) {
				 
				insn->imm = 0;
				insn->code |= BPF_X;
				continue;
			}

			fn = prog->aux->ops->get_func_proto(insn->imm);
			 
			BUG_ON(!fn->func);
			insn->imm = fn->func - __bpf_call_base;
		}
	}
}