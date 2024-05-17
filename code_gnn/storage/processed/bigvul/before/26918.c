static void fill_stats(struct task_struct *tsk, struct taskstats *stats)
{
	memset(stats, 0, sizeof(*stats));
	 

	delayacct_add_tsk(stats, tsk);

	 
	stats->version = TASKSTATS_VERSION;
	stats->nvcsw = tsk->nvcsw;
	stats->nivcsw = tsk->nivcsw;
	bacct_add_tsk(stats, tsk);

	 
	xacct_add_tsk(stats, tsk);
}
