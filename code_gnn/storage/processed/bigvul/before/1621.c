assegment_normalise (struct assegment *head)
{
  struct assegment *seg = head, *pin;
  struct assegment *tmp;
  
  if (!head)
    return head;
  
  while (seg)
    {
      pin = seg;
      
       
      if (seg->type == AS_SET || seg->type == AS_CONFED_SET)
      	{
	  int tail = 0;
	  int i;
	  
	  qsort (seg->as, seg->length, sizeof(as_t), int_cmp);
	  
	   
	  for (i=1; i < seg->length; i++)
	    {
	      if (seg->as[tail] == seg->as[i])
	      	continue;
	      
	      tail++;
	      if (tail < i)
	      	seg->as[tail] = seg->as[i];
	    }
	   
	  if (seg->length)
	    seg->length = tail + 1;
	}

             
      while (pin->next && ASSEGMENT_TYPES_PACKABLE(pin, pin->next))
        {
          tmp = pin->next;
          seg = pin->next;
          
           
          pin = assegment_append_asns (pin, seg->as, seg->length);
          
           
          pin->next = seg->next;
          
           
          assegment_free (tmp);
          
        }

      seg = pin->next;
    }
  return head;
}
