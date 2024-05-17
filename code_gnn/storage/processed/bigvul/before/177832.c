 walk_string(fz_context *ctx, int uni, int remove, editable_str *str)
  {
         int rune;
  
       if (str->utf8 == NULL)
//        if (str->utf8 == NULL || str->pos == -1)
                 return;
  
         do
 	{
 		char *s = &str->utf8[str->pos];
 		size_t len;
 		int n = fz_chartorune(&rune, s);
 		if (rune == uni)
 		{
 			 
 			str->pos += n;
 		}
 		else if (uni == 32) {
 			 
 			break;
 		}
 		else if (rune == 32) {
 			 
 		}
 		else
 		{
 			 
 			str->pos = -1;
 			break;
 		}
 		if (remove)
 		{
 			len = strlen(s+n);
 			memmove(s, s+n, len+1);
 			str->edited = 1;
 		}
 	}
 	while (rune != uni);
 }