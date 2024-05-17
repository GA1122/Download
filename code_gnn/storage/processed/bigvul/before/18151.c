kick_delayed_job(tube t)
{
    int r;
    job j;
    size_t z;

    j = pq_take(&t->delay);
    if (!j) return 0;

    z = binlog_reserve_space_update(j);
    if (!z) return pq_give(&t->delay, j), 0;  
    j->reserved_binlog_space += z;

    j->kick_ct++;
    r = enqueue_job(j, 0, 1);
    if (r == 1) return 1;

     
    r = enqueue_job(j, j->delay, 0);
    if (r == 1) return 0;

     
    bury_job(j, 0);
    return 0;
}
