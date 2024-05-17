bus_activation_new (BusContext        *context,
                    const DBusString  *address,
                    DBusList         **directories,
                    DBusError         *error)
{
  BusActivation *activation;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  activation = dbus_new0 (BusActivation, 1);
  if (activation == NULL)
    {
      BUS_SET_OOM (error);
      return NULL;
    }

  activation->refcount = 1;
  activation->context = context;
  activation->n_pending_activations = 0;

  if (!bus_activation_reload (activation, address, directories, error))
    goto failed;

    
  activation->pending_activations = _dbus_hash_table_new (DBUS_HASH_STRING, NULL,
                                                          (DBusFreeFunction)bus_pending_activation_unref);

  if (activation->pending_activations == NULL)
    {
      BUS_SET_OOM (error);
      goto failed;
    }

  activation->environment = _dbus_hash_table_new (DBUS_HASH_STRING,
                                                  (DBusFreeFunction) dbus_free,
                                                  (DBusFreeFunction) dbus_free);

  if (activation->environment == NULL)
    {
      BUS_SET_OOM (error);
      goto failed;
    }

  if (!populate_environment (activation))
    {
      BUS_SET_OOM (error);
      goto failed;
    }

  return activation;

 failed:
  bus_activation_unref (activation);
  return NULL;
}
