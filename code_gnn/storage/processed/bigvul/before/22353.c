static void calc_load_account_idle(struct rq *this_rq)
{
	long delta;

	delta = calc_load_fold_active(this_rq);
	if (delta)
		atomic_long_add(delta, &calc_load_tasks_idle);
}
