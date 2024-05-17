_bus_service_find_owner_link (BusService *service,
                              DBusConnection *connection)
{
  DBusList *link;
  
  link = _dbus_list_get_first_link (&service->owners);

  while (link != NULL)
    {
      BusOwner *bus_owner;

      bus_owner = (BusOwner *) link->data;
      if (bus_owner->conn == connection) 
        break;

      link = _dbus_list_get_next_link (&service->owners, link);
    }

  return link;
}
