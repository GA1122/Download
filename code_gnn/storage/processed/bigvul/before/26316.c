static inline void prepare_lock_switch(struct rq *rq, struct task_struct *next)
{
#ifdef CONFIG_SMP
	 
	next->on_cpu = 1;
#endif
#ifdef __ARCH_WANT_INTERRUPTS_ON_CTXSW
	raw_spin_unlock_irq(&rq->lock);
#else
	raw_spin_unlock(&rq->lock);
#endif
}
