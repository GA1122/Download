 transform_name(int t)
   
 {
  unsigned int i;
 
  
     t &= -t;  
  
   for (i=0; i<TTABLE_SIZE; ++i)
//    for (i=0; i<TTABLE_SIZE; ++i) if (transform_info[i].name != NULL)
     {
        if ((transform_info[i].transform & t) != 0)
           return transform_info[i].name;
  }
 
  return "invalid transform";
 }