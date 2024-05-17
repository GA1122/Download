static inline void prepare_lock_switch(struct rq *rq, struct task_struct *next)
{
#ifdef CONFIG_SMP
	 
	next->on_cpu = 1;
#endif
}
