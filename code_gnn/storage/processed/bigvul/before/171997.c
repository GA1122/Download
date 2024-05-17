static void initialize_transaction(int lbl)
{
    pthread_mutex_lock(&device.lbllock);
 if(lbl < MAX_TRANSACTIONS_PER_SESSION)
 {
       device.transaction[lbl].lbl = lbl;
       device.transaction[lbl].in_use=FALSE;
       device.transaction[lbl].handle=0;
 }
    pthread_mutex_unlock(&device.lbllock);
}
