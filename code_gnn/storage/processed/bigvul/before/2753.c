_dbus_header_load (DBusHeader        *header,
                   DBusValidationMode mode,
                   DBusValidity      *validity,
                   int                byte_order,
                   int                fields_array_len,
                   int                header_len,
                   int                body_len,
                   const DBusString  *str,
                   int                start,
                   int                len)
{
  int leftover;
  DBusValidity v;
  DBusTypeReader reader;
  DBusTypeReader array_reader;
  unsigned char v_byte;
  dbus_uint32_t v_uint32;
  dbus_uint32_t serial;
  int padding_start;
  int padding_len;
  int i;

  _dbus_assert (start == (int) _DBUS_ALIGN_VALUE (start, 8));
  _dbus_assert (header_len <= len);
  _dbus_assert (_dbus_string_get_length (&header->data) == 0);

  if (!_dbus_string_copy_len (str, start, header_len, &header->data, 0))
    {
      _dbus_verbose ("Failed to copy buffer into new header\n");
      *validity = DBUS_VALIDITY_UNKNOWN_OOM_ERROR;
      return FALSE;
    }

  if (mode == DBUS_VALIDATION_MODE_WE_TRUST_THIS_DATA_ABSOLUTELY)
    {
      leftover = len - header_len - body_len - start;
    }
  else
    {
      v = _dbus_validate_body_with_reason (&_dbus_header_signature_str, 0,
                                           byte_order,
                                           &leftover,
                                           str, start, len);
      
      if (v != DBUS_VALID)
        {
          *validity = v;
          goto invalid;
        }
    }

  _dbus_assert (leftover < len);

  padding_len = header_len - (FIRST_FIELD_OFFSET + fields_array_len);
  padding_start = start + FIRST_FIELD_OFFSET + fields_array_len;
  _dbus_assert (start + header_len == (int) _DBUS_ALIGN_VALUE (padding_start, 8));
  _dbus_assert (start + header_len == padding_start + padding_len);

  if (mode != DBUS_VALIDATION_MODE_WE_TRUST_THIS_DATA_ABSOLUTELY)
    {
      if (!_dbus_string_validate_nul (str, padding_start, padding_len))
        {
          *validity = DBUS_INVALID_ALIGNMENT_PADDING_NOT_NUL;
          goto invalid;
        }
    }

  header->padding = padding_len;

  if (mode == DBUS_VALIDATION_MODE_WE_TRUST_THIS_DATA_ABSOLUTELY)
    {
      *validity = DBUS_VALID;
      return TRUE;
    }

   

  _dbus_type_reader_init (&reader,
                          byte_order,
                          &_dbus_header_signature_str, 0,
                          str, start);

   
  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_BYTE);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == BYTE_ORDER_OFFSET);
  _dbus_type_reader_read_basic (&reader, &v_byte);
  _dbus_type_reader_next (&reader);

  _dbus_assert (v_byte == byte_order);
  header->byte_order = byte_order;

   
  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_BYTE);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == TYPE_OFFSET);
  _dbus_type_reader_read_basic (&reader, &v_byte);
  _dbus_type_reader_next (&reader);

   
  if (v_byte == DBUS_MESSAGE_TYPE_INVALID)
    {
      *validity = DBUS_INVALID_BAD_MESSAGE_TYPE;
      goto invalid;
    }

   
  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_BYTE);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == FLAGS_OFFSET);
  _dbus_type_reader_read_basic (&reader, &v_byte);
  _dbus_type_reader_next (&reader);

   

   
  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_BYTE);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == VERSION_OFFSET);
  _dbus_type_reader_read_basic (&reader, &v_byte);
  _dbus_type_reader_next (&reader);

  if (v_byte != DBUS_MAJOR_PROTOCOL_VERSION)
    {
      *validity = DBUS_INVALID_BAD_PROTOCOL_VERSION;
      goto invalid;
    }

   
  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_UINT32);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == BODY_LENGTH_OFFSET);
  _dbus_type_reader_read_basic (&reader, &v_uint32);
  _dbus_type_reader_next (&reader);

  _dbus_assert (body_len == (signed) v_uint32);

   
  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_UINT32);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == SERIAL_OFFSET);
  _dbus_type_reader_read_basic (&reader, &serial);
  _dbus_type_reader_next (&reader);

  if (serial == 0)
    {
      *validity = DBUS_INVALID_BAD_SERIAL;
      goto invalid;
    }

  _dbus_assert (_dbus_type_reader_get_current_type (&reader) == DBUS_TYPE_ARRAY);
  _dbus_assert (_dbus_type_reader_get_value_pos (&reader) == FIELDS_ARRAY_LENGTH_OFFSET);

  _dbus_type_reader_recurse (&reader, &array_reader);
  while (_dbus_type_reader_get_current_type (&array_reader) != DBUS_TYPE_INVALID)
    {
      DBusTypeReader struct_reader;
      DBusTypeReader variant_reader;
      unsigned char field_code;

      _dbus_assert (_dbus_type_reader_get_current_type (&array_reader) == DBUS_TYPE_STRUCT);

      _dbus_type_reader_recurse (&array_reader, &struct_reader);

      _dbus_assert (_dbus_type_reader_get_current_type (&struct_reader) == DBUS_TYPE_BYTE);
      _dbus_type_reader_read_basic (&struct_reader, &field_code);
      _dbus_type_reader_next (&struct_reader);

      if (field_code == DBUS_HEADER_FIELD_INVALID)
        {
          _dbus_verbose ("invalid header field code\n");
          *validity = DBUS_INVALID_HEADER_FIELD_CODE;
          goto invalid;
        }

      if (field_code > DBUS_HEADER_FIELD_LAST)
        {
          _dbus_verbose ("unknown header field code %d, skipping\n",
                         field_code);
          goto next_field;
        }

      _dbus_assert (_dbus_type_reader_get_current_type (&struct_reader) == DBUS_TYPE_VARIANT);
      _dbus_type_reader_recurse (&struct_reader, &variant_reader);

      v = load_and_validate_field (header, field_code, &variant_reader);
      if (v != DBUS_VALID)
        {
          _dbus_verbose ("Field %d was invalid\n", field_code);
          *validity = v;
          goto invalid;
        }

    next_field:
      _dbus_type_reader_next (&array_reader);
    }

   
  i = 0;
  while (i <= DBUS_HEADER_FIELD_LAST)
    {
      if (header->fields[i].value_pos == _DBUS_HEADER_FIELD_VALUE_UNKNOWN)
        header->fields[i].value_pos = _DBUS_HEADER_FIELD_VALUE_NONEXISTENT;
      ++i;
    }

  v = check_mandatory_fields (header);
  if (v != DBUS_VALID)
    {
      _dbus_verbose ("Mandatory fields were missing, code %d\n", v);
      *validity = v;
      goto invalid;
    }

  *validity = DBUS_VALID;
  return TRUE;

 invalid:
  _dbus_string_set_length (&header->data, 0);
  return FALSE;
}
