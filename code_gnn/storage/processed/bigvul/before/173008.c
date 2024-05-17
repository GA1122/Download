buffer_init(struct buffer *buffer)
  
{
   buffer->first.next = NULL;
   buffer->last = NULL;

    buffer->current = NULL;
 }
