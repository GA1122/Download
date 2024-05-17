static size_t add_data_segment(u_char **data, size_t base, u_char **string, size_t slen)
{
    
   SAFE_REALLOC(*data, base + slen + 1);

    
   memcpy(*data + base, *string, slen + 1);

    
   *string = (u_char *)base;
   
    
   return slen + 1;
}
