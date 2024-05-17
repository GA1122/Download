add_restore_pending_to_transaction (BusTransaction       *transaction,
                                    BusPendingActivation *pending_activation)
{
  RestorePendingData *d;

  d = dbus_new (RestorePendingData, 1);
  if (d == NULL)
    return FALSE;

  d->pending_activation = pending_activation;
  d->hash_entry = _dbus_hash_table_preallocate_entry (d->pending_activation->activation->pending_activations);

  bus_pending_activation_ref (d->pending_activation);

  if (d->hash_entry == NULL ||
      !bus_transaction_add_cancel_hook (transaction, restore_pending, d,
                                        free_pending_restore_data))
    {
      free_pending_restore_data (d);
      return FALSE;
    }

  _dbus_verbose ("Saved pending activation to be restored if the transaction fails\n");

  return TRUE;
}
