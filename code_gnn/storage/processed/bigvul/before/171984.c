rc_transaction_t *get_transaction_by_lbl(UINT8 lbl)
{
 rc_transaction_t *transaction = NULL;
    pthread_mutex_lock(&device.lbllock);

  
 if (lbl < MAX_TRANSACTIONS_PER_SESSION)
 {
 if (FALSE==device.transaction[lbl].in_use)
 {
            transaction = NULL;
 }
 else
 {
            transaction = &(device.transaction[lbl]);
            BTIF_TRACE_DEBUG("%s: Got transaction.label: %d",__FUNCTION__,lbl);
 }
 }

    pthread_mutex_unlock(&device.lbllock);
 return transaction;
}
