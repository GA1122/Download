asmlinkage void do_debug_interrupt(unsigned long code, struct pt_regs *regs)
{
	u64 peek_real_address_q(u64 addr);
	u64 poke_real_address_q(u64 addr, u64 val);
	unsigned long long DM_EXP_CAUSE_PHY = 0x0c100010;
	unsigned long long exp_cause;
	 
	exp_cause = peek_real_address_q(DM_EXP_CAUSE_PHY);
	if (exp_cause & ~4) {
		printk("DM.EXP_CAUSE had unexpected bits set (=%08lx)\n",
			(unsigned long)(exp_cause & 0xffffffff));
	}
	show_state();
	 
	poke_real_address_q(DM_EXP_CAUSE_PHY, 0x0);
}