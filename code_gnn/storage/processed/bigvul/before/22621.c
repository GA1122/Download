static inline int task_running(struct rq *rq, struct task_struct *p)
{
	return task_current(rq, p);
}
