buffer_write(struct display *dp, struct buffer *buffer, png_bytep data,
 png_size_t size)
  
{
  
 struct buffer_list *last = buffer->last;
 size_t end_count = buffer->end_count;

 while (size > 0)
 {
 size_t avail;

 if (end_count >= sizeof last->buffer)
 {
 if (last->next == NULL)
 {
            last = buffer_extend(last);

 if (last == NULL)
               display_log(dp, APP_ERROR, "out of memory saving file");
 }

 else
            last = last->next;

         buffer->last = last;  
         end_count = 0;
 }

      avail = (sizeof last->buffer) - end_count;
 if (avail > size)
         avail = size;

      memcpy(last->buffer + end_count, data, avail);
      end_count += avail;
      size -= avail;
      data += avail;
 }

   buffer->end_count = end_count;
}
