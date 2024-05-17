static inline void advance(struct pt_regs *regs)
{
	regs->pc   = regs->npc;
	regs->npc += 4;
}
