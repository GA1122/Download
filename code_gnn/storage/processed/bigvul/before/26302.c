static void irqtime_account_idle_ticks(int ticks)
{
	int i;
	struct rq *rq = this_rq();

	for (i = 0; i < ticks; i++)
		irqtime_account_process_tick(current, 0, rq);
}
