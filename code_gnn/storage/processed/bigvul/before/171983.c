bt_status_t  get_transaction(rc_transaction_t **ptransaction)
{
 bt_status_t result = BT_STATUS_NOMEM;
    UINT8 i=0;
    pthread_mutex_lock(&device.lbllock);

 for (i=0; i<MAX_TRANSACTIONS_PER_SESSION; i++)
 {
 if (FALSE==device.transaction[i].in_use)
 {
            BTIF_TRACE_DEBUG("%s:Got transaction.label: %d",__FUNCTION__,device.transaction[i].lbl);
            device.transaction[i].in_use = TRUE;
 *ptransaction = &(device.transaction[i]);
            result = BT_STATUS_SUCCESS;
 break;
 }
 }

    pthread_mutex_unlock(&device.lbllock);
 return result;
}
