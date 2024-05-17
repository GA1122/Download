void StreamTcpSessionClear(void *ssnptr)
{
    SCEnter();
    TcpSession *ssn = (TcpSession *)ssnptr;
    if (ssn == NULL)
        return;

    StreamTcpSessionCleanup(ssn);

     
    PoolThreadReserved a = ssn->res;
    memset(ssn, 0, sizeof(TcpSession));
    ssn->res = a;

    PoolThreadReturn(ssn_pool, ssn);
#ifdef DEBUG
    SCMutexLock(&ssn_pool_mutex);
    ssn_pool_cnt--;
    SCMutexUnlock(&ssn_pool_mutex);
#endif

    SCReturn;
}