bus_owner_new (BusService *service, 
               DBusConnection *conn, 
	       dbus_uint32_t flags)
{
  BusOwner *result;

  result = _dbus_mem_pool_alloc (service->registry->owner_pool);
  if (result != NULL)
    {
      result->refcount = 1;
       
      result->conn = conn;
      result->service = service;

      if (!bus_connection_add_owned_service (conn, service))
        {
          _dbus_mem_pool_dealloc (service->registry->owner_pool, result);
          return NULL;
        }
        
      bus_owner_set_flags (result, flags);
    }
  return result;
}
