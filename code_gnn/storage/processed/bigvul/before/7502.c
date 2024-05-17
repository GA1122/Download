set_reply_serial (DBusMessage *message)
{
  if (message == NULL)
    _dbus_assert_not_reached ("oom");
  if (!dbus_message_set_reply_serial (message, 100))
    _dbus_assert_not_reached ("oom");
}
