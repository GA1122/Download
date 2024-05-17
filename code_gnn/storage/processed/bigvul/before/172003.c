void release_transaction(UINT8 lbl)
{
 rc_transaction_t *transaction = get_transaction_by_lbl(lbl);

  
 if (transaction != NULL)
 {
        BTIF_TRACE_DEBUG("%s: lbl: %d", __FUNCTION__, lbl);
        initialize_transaction(lbl);
 }
}
