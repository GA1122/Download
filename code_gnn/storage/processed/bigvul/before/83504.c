void ist_enter(struct pt_regs *regs)
{
	if (user_mode(regs)) {
		RCU_LOCKDEP_WARN(!rcu_is_watching(), "entry code didn't wake RCU");
	} else {
		 
		rcu_nmi_enter();
	}

	preempt_disable();

	 
	RCU_LOCKDEP_WARN(!rcu_is_watching(), "ist_enter didn't work");
}
