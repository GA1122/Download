parse_header (Buffer *buffer, guint32 serial_offset, guint32 reply_serial_offset, guint32 hello_serial)
{
  guint32 array_len, header_len;
  guint32 offset, end_offset;
  guint8 header_type;
  guint32 reply_serial_pos = 0;
  const char *signature;
  g_autoptr(Header) header = g_new0 (Header, 1);

  header->buffer = buffer_ref (buffer);

  if (buffer->size < 16)
    return NULL;

  if (buffer->data[3] != 1)  
    return NULL;

  if (buffer->data[0] == 'B')
    header->big_endian = TRUE;
  else if (buffer->data[0] == 'l')
    header->big_endian = FALSE;
  else
    return NULL;

  header->type = buffer->data[1];
  header->flags = buffer->data[2];

  header->length = read_uint32 (header, &buffer->data[4]);
  header->serial = read_uint32 (header, &buffer->data[8]);

  if (header->serial == 0)
    return NULL;

  array_len = read_uint32 (header, &buffer->data[12]);

  header_len = align_by_8 (12 + 4 + array_len);
  g_assert (buffer->size >= header_len);  
  if (header_len > buffer->size)
    return NULL;

  offset = 12 + 4;
  end_offset = offset + array_len;

  while (offset < end_offset)
    {
      offset = align_by_8 (offset);  
      if (offset >= end_offset)
        return NULL;

      header_type = buffer->data[offset++];
      if (offset >= end_offset)
        return NULL;

      signature = get_signature (buffer, &offset, end_offset);
      if (signature == NULL)
        return NULL;

      switch (header_type)
        {
        case G_DBUS_MESSAGE_HEADER_FIELD_INVALID:
          return NULL;

        case G_DBUS_MESSAGE_HEADER_FIELD_PATH:
          if (strcmp (signature, "o") != 0)
            return NULL;
          header->path = get_string (buffer, header, &offset, end_offset);
          if (header->path == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_INTERFACE:
          if (strcmp (signature, "s") != 0)
            return NULL;
          header->interface = get_string (buffer, header, &offset, end_offset);
          if (header->interface == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_MEMBER:
          if (strcmp (signature, "s") != 0)
            return NULL;
          header->member = get_string (buffer, header, &offset, end_offset);
          if (header->member == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_ERROR_NAME:
          if (strcmp (signature, "s") != 0)
            return NULL;
          header->error_name = get_string (buffer, header, &offset, end_offset);
          if (header->error_name == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_REPLY_SERIAL:
          if (offset + 4 > end_offset)
            return NULL;

          header->has_reply_serial = TRUE;
          reply_serial_pos = offset;
          header->reply_serial = read_uint32 (header, &buffer->data[offset]);
          offset += 4;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_DESTINATION:
          if (strcmp (signature, "s") != 0)
            return NULL;
          header->destination = get_string (buffer, header, &offset, end_offset);
          if (header->destination == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_SENDER:
          if (strcmp (signature, "s") != 0)
            return NULL;
          header->sender = get_string (buffer, header, &offset, end_offset);
          if (header->sender == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_SIGNATURE:
          if (strcmp (signature, "g") != 0)
            return NULL;
          header->signature = get_signature (buffer, &offset, end_offset);
          if (header->signature == NULL)
            return NULL;
          break;

        case G_DBUS_MESSAGE_HEADER_FIELD_NUM_UNIX_FDS:
          if (offset + 4 > end_offset)
            return NULL;

          header->unix_fds = read_uint32 (header, &buffer->data[offset]);
          offset += 4;
          break;

        default:
           
          return NULL;
        }
    }

  switch (header->type)
    {
    case G_DBUS_MESSAGE_TYPE_METHOD_CALL:
      if (header->path == NULL || header->member == NULL)
        return NULL;
      break;

    case G_DBUS_MESSAGE_TYPE_METHOD_RETURN:
      if (!header->has_reply_serial)
        return NULL;
      break;

    case G_DBUS_MESSAGE_TYPE_ERROR:
      if (header->error_name  == NULL || !header->has_reply_serial)
        return NULL;
      break;

    case G_DBUS_MESSAGE_TYPE_SIGNAL:
      if (header->path == NULL ||
          header->interface == NULL ||
          header->member == NULL)
        return NULL;
      if (strcmp (header->path, "/org/freedesktop/DBus/Local") == 0 ||
          strcmp (header->interface, "org.freedesktop.DBus.Local") == 0)
        return NULL;
      break;

    default:
       
      return NULL;
    }

  if (serial_offset > 0)
    {
      header->serial += serial_offset;
      write_uint32 (header, &buffer->data[8], header->serial);
    }

  if (reply_serial_offset > 0 &&
      header->has_reply_serial &&
      header->reply_serial > hello_serial + reply_serial_offset)
    write_uint32 (header, &buffer->data[reply_serial_pos], header->reply_serial - reply_serial_offset);

  return g_steal_pointer (&header);
}