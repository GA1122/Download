bool sched_can_stop_tick(void)
{
       struct rq *rq;

       rq = this_rq();

        
       smp_rmb();

        
       if (rq->nr_running > 1)
               return false;

       return true;
}
