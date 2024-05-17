aspath_prepend (struct aspath *as1, struct aspath *as2)
{
  struct assegment *seg1;
  struct assegment *seg2;

  if (! as1 || ! as2)
    return NULL;
  
  seg1 = as1->segments;
  seg2 = as2->segments;
  
   
  if (seg2 == NULL)
    {
      as2->segments = assegment_dup_all (as1->segments);
      aspath_str_update (as2);
      return as2;
    }
  
   
  if (seg1 == NULL)
    return as2;
  
   
  while (seg1 && seg1->next)
    seg1 = seg1->next;

   
  if (seg1->type == AS_SEQUENCE && seg2->type == AS_CONFED_SEQUENCE)
    as2 = aspath_delete_confed_seq (as2);
  
   
  seg2 = as2->segments;
  
   
  if (seg2 == NULL)
    {
      as2->segments = assegment_dup_all (as1->segments);
      aspath_str_update (as2);
      return as2;
    }
  
   
  if (seg1->type != seg2->type)
    return aspath_merge (as1, as2);

  if (seg1->type == AS_SEQUENCE)
    {
       
      
       
      seg1 = as2->segments = assegment_dup_all (as1->segments);
      
       
      while (seg1 && seg1->next)
        seg1 = seg1->next;
      
       
      seg1 = assegment_append_asns (seg1, seg2->as, seg2->length);
      
       
      seg1->next = seg2->next;
      
       
      assegment_free (seg2);
      
       
      aspath_str_update (as2);
      return as2;
    }
  else
    {
       
      return aspath_merge (as1, as2);
    }
   
  
   
}
