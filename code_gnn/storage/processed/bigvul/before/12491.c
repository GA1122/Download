add_bus_environment (BusActivation *activation,
                     DBusError     *error)
{
  const char *type;

  if (!bus_activation_set_environment_variable (activation,
                                                "DBUS_STARTER_ADDRESS",
                                                activation->server_address,
                                                error))
    return FALSE;

  type = bus_context_get_type (activation->context);
  if (type != NULL)
    {
      if (!bus_activation_set_environment_variable (activation,
                                                    "DBUS_STARTER_BUS_TYPE", type,
                                                    error))
        return FALSE;

      if (strcmp (type, "session") == 0)
        {
          if (!bus_activation_set_environment_variable (activation,
                                                        "DBUS_SESSION_BUS_ADDRESS",
                                                        activation->server_address,
                                                        error))
            return FALSE;
        }
      else if (strcmp (type, "system") == 0)
        {
          if (!bus_activation_set_environment_variable (activation,
                                                        "DBUS_SYSTEM_BUS_ADDRESS",
                                                        activation->server_address,
                                                        error))
            return FALSE;
        }
    }

  return TRUE;
}
