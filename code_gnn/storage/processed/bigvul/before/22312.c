static inline void __set_task_cpu(struct task_struct *p, unsigned int cpu)
{
	set_task_rq(p, cpu);
#ifdef CONFIG_SMP
	 
	smp_wmb();
	task_thread_info(p)->cpu = cpu;
#endif
}
