load_and_validate_field (DBusHeader     *header,
                         int             field,
                         DBusTypeReader *variant_reader)
{
  int type;
  int expected_type;
  const DBusString *value_str;
  int value_pos;
  int str_data_pos;
  dbus_uint32_t v_UINT32;
  int bad_string_code;
  dbus_bool_t (* string_validation_func) (const DBusString *str,
                                          int start, int len);

   
  _dbus_assert (field <= DBUS_HEADER_FIELD_LAST);
  _dbus_assert (field != DBUS_HEADER_FIELD_INVALID);

   
  type = _dbus_type_reader_get_current_type (variant_reader);

  _dbus_assert (_dbus_header_field_types[field].code == field);

  expected_type = EXPECTED_TYPE_OF_FIELD (field);
  if (type != expected_type)
    {
      _dbus_verbose ("Field %d should have type %d but has %d\n",
                     field, expected_type, type);
      return DBUS_INVALID_HEADER_FIELD_HAS_WRONG_TYPE;
    }

   
  if (header->fields[field].value_pos >= 0)
    {
      _dbus_verbose ("Header field %d seen a second time\n", field);
      return DBUS_INVALID_HEADER_FIELD_APPEARS_TWICE;
    }

   
  _dbus_verbose ("initially caching field %d\n", field);
  _dbus_header_cache_one (header, field, variant_reader);

  string_validation_func = NULL;

   
  v_UINT32 = 0;
  value_str = NULL;
  value_pos = -1;
  str_data_pos = -1;
  bad_string_code = DBUS_VALID;

  if (expected_type == DBUS_TYPE_UINT32)
    {
      _dbus_header_get_field_basic (header, field, expected_type,
                                    &v_UINT32);
    }
  else if (expected_type == DBUS_TYPE_STRING ||
           expected_type == DBUS_TYPE_OBJECT_PATH ||
           expected_type == DBUS_TYPE_SIGNATURE)
    {
      _dbus_header_get_field_raw (header, field,
                                  &value_str, &value_pos);
      str_data_pos = _DBUS_ALIGN_VALUE (value_pos, 4) + 4;
    }
  else
    {
      _dbus_assert_not_reached ("none of the known fields should have this type");
    }

  switch (field)
    {
    case DBUS_HEADER_FIELD_DESTINATION:
      string_validation_func = _dbus_validate_bus_name;
      bad_string_code = DBUS_INVALID_BAD_DESTINATION;
      break;
    case DBUS_HEADER_FIELD_INTERFACE:
      string_validation_func = _dbus_validate_interface;
      bad_string_code = DBUS_INVALID_BAD_INTERFACE;

      if (_dbus_string_equal_substring (&_dbus_local_interface_str,
                                        0,
                                        _dbus_string_get_length (&_dbus_local_interface_str),
                                        value_str, str_data_pos))
        {
          _dbus_verbose ("Message is on the local interface\n");
          return DBUS_INVALID_USES_LOCAL_INTERFACE;
        }
      break;

    case DBUS_HEADER_FIELD_MEMBER:
      string_validation_func = _dbus_validate_member;
      bad_string_code = DBUS_INVALID_BAD_MEMBER;
      break;

    case DBUS_HEADER_FIELD_ERROR_NAME:
      string_validation_func = _dbus_validate_error_name;
      bad_string_code = DBUS_INVALID_BAD_ERROR_NAME;
      break;

    case DBUS_HEADER_FIELD_SENDER:
      string_validation_func = _dbus_validate_bus_name;
      bad_string_code = DBUS_INVALID_BAD_SENDER;
      break;

    case DBUS_HEADER_FIELD_PATH:
       
      string_validation_func = NULL;

      if (_dbus_string_equal_substring (&_dbus_local_path_str,
                                        0,
                                        _dbus_string_get_length (&_dbus_local_path_str),
                                        value_str, str_data_pos))
        {
          _dbus_verbose ("Message is from the local path\n");
          return DBUS_INVALID_USES_LOCAL_PATH;
        }
      break;

    case DBUS_HEADER_FIELD_REPLY_SERIAL:
       
      if (v_UINT32 == 0)
        {
          return DBUS_INVALID_BAD_SERIAL;
        }
      break;

    case DBUS_HEADER_FIELD_SIGNATURE:
       
      string_validation_func = NULL;
      break;

    default:
      _dbus_assert_not_reached ("unknown field shouldn't be seen here");
      break;
    }

  if (string_validation_func)
    {
      dbus_uint32_t len;

      _dbus_assert (bad_string_code != DBUS_VALID);

      len = _dbus_marshal_read_uint32 (value_str, value_pos,
                                       header->byte_order, NULL);

#if 0
      _dbus_verbose ("Validating string header field; code %d if fails\n",
                     bad_string_code);
#endif
      if (!(*string_validation_func) (value_str, str_data_pos, len))
        return bad_string_code;
    }

  return DBUS_VALID;
}
