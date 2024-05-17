rechunk_length(struct IDAT *idat)
  
{
   png_uint_32 len = idat->global->idat_max;

 if (len == 0)  
 {
 const struct IDAT_list *cur;
 unsigned int count;

 if (idat->idat_index == 0)  
 return idat->idat_length;  

  
      cur = idat->idat_cur;
      count = idat->idat_count;

      assert(idat->idat_index == idat->idat_length &&
         idat->idat_length == cur->lengths[count]);

  
 if (++count < cur->count)
 return cur->lengths[count];

  
      assert(cur != idat->idat_list_tail);
      cur = cur->next;
      assert(cur != NULL && cur->count > 0);
 return cur->lengths[0];
 }

 else  
 {
  
      png_uint_32 have = idat->idat_length - idat->idat_index;

 if (len > have)
 {
 struct IDAT_list *cur = idat->idat_cur;
 unsigned int j = idat->idat_count+1;  

 do
 {
  
            assert(cur != NULL);

 for (;;)
 {
  
 for (; j < cur->count; ++j)
 {
                  have += cur->lengths[j];
 if (len <= have)
 return len;
 }

  
 if (cur == idat->idat_list_tail)
 return have;

               cur = cur->next;
               j = 0;
 }
 }
 while (len > have);
 }

 return len;
 }
}
