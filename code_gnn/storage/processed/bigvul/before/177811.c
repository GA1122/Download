 aspath_put (struct stream *s, struct aspath *as, int use32bit )
 {
   struct assegment *seg = as->segments;
   size_t bytes = 0;
   
   if (!seg || seg->length == 0)
     return 0;
   
   if (seg)
     {
        
       while (seg && (ASSEGMENT_LEN(seg, use32bit) <= STREAM_WRITEABLE(s)))
         {
           struct assegment *next = seg->next;
           int written = 0;
           int asns_packed = 0;
           size_t lenp;
           
            
           while ( (seg->length - written) > AS_SEGMENT_MAX)
             {
                assegment_header_put (s, seg->type, AS_SEGMENT_MAX);
                assegment_data_put (s, seg->as, AS_SEGMENT_MAX, use32bit);
                written += AS_SEGMENT_MAX;
              bytes += ASSEGMENT_SIZE (written, use32bit);
//               bytes += ASSEGMENT_SIZE (AS_SEGMENT_MAX, use32bit);
              }
            
             
           lenp = assegment_header_put (s, seg->type, seg->length - written);
           assegment_data_put (s, (seg->as + written), seg->length - written, 
                               use32bit);
           
            
           while (next && ASSEGMENTS_PACKABLE (seg, next))
             {
                
               
                
               assegment_data_put (s, next->as, next->length, use32bit);
               
                
 	      stream_putc_at (s, lenp, seg->length - written + next->length);
               asns_packed += next->length;
                
 	      next = next->next;
 	    }
           
           bytes += ASSEGMENT_SIZE (seg->length - written + asns_packed, 
 				   use32bit);
           seg = next;
         }
     }
   return bytes;
 }