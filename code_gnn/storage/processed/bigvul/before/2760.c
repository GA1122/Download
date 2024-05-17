check_mandatory_fields (DBusHeader *header)
{
#define REQUIRE_FIELD(name) do { if (header->fields[DBUS_HEADER_FIELD_##name].value_pos < 0) return DBUS_INVALID_MISSING_##name; } while (0)

  switch (_dbus_header_get_message_type (header))
    {
    case DBUS_MESSAGE_TYPE_SIGNAL:
      REQUIRE_FIELD (INTERFACE);
       
    case DBUS_MESSAGE_TYPE_METHOD_CALL:
      REQUIRE_FIELD (PATH);
      REQUIRE_FIELD (MEMBER);
      break;
    case DBUS_MESSAGE_TYPE_ERROR:
      REQUIRE_FIELD (ERROR_NAME);
      REQUIRE_FIELD (REPLY_SERIAL);
      break;
    case DBUS_MESSAGE_TYPE_METHOD_RETURN:
      REQUIRE_FIELD (REPLY_SERIAL);
      break;
    default:
       
      break;
    }

  return DBUS_VALID;
}
