buffer_new (gsize size, Buffer *old)
{
  Buffer *buffer = g_malloc0 (sizeof (Buffer) + size - 16);

  buffer->control_messages = NULL;
  buffer->size = size;
  buffer->refcount = 1;

  if (old)
    {
      buffer->pos = old->pos;
       
      buffer->control_messages = old->control_messages;
      old->control_messages = NULL;

      g_assert (size >= old->size);
      memcpy (buffer->data, old->data, old->size);
    }

  return buffer;
}