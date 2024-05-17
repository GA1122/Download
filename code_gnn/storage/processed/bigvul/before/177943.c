 bmexec_trans (kwset_t kwset, char const *text, size_t size)
 {
   unsigned char const *d1;
   char const *ep, *sp, *tp;
   int d;
   int len = kwset->mind;
   char const *trans = kwset->trans;
 
   if (len == 0)
     return 0;
   if (len > size)
     return -1;
   if (len == 1)
     {
       tp = memchr_kwset (text, size, kwset);
       return tp ? tp - text : -1;
     }
 
   d1 = kwset->delta;
   sp = kwset->target + len;
   tp = text + len;
   char gc1 = kwset->gc1;
   char gc2 = kwset->gc2;
 
    
   if (size > 12 * len)
      
     for (ep = text + size - 11 * len; tp <= ep; )
       {
         char const *tp0 = tp;
         d = d1[U(tp[-1])], tp += d;
         d = d1[U(tp[-1])], tp += d;
         if (d != 0)
           {
             d = d1[U(tp[-1])], tp += d;
             d = d1[U(tp[-1])], tp += d;
             d = d1[U(tp[-1])], tp += d;
             if (d != 0)
               {
                 d = d1[U(tp[-1])], tp += d;
                 d = d1[U(tp[-1])], tp += d;
                 d = d1[U(tp[-1])], tp += d;
                 if (d != 0)
                   {
                     d = d1[U(tp[-1])], tp += d;
                     d = d1[U(tp[-1])], tp += d;
 
                      
                     int advance_heuristic = 16 * sizeof (long);
                     if (advance_heuristic <= tp - tp0)
                       goto big_advance;
                     tp--;
                     tp = memchr_kwset (tp, text + size - tp, kwset);
                      if (! tp)
                        return -1;
                      tp++;
//                     if (ep <= tp)
//                       break;
                    }
                }
            }
       big_advance:;
       }
 
    
   ep = text + size;
   d = d1[U(tp[-1])];
   while (d <= ep - tp)
     {
       d = d1[U((tp += d)[-1])];
       if (d != 0)
         continue;
       if (bm_delta2_search (&tp, ep, sp, len, trans, gc1, gc2, NULL, kwset))
         return tp - text;
     }
 
   return -1;
 }