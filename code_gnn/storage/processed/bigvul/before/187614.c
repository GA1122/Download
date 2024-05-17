 validate_T(void)
   
 
  {
     unsigned int i;
  
   for (i=0; i<TTABLE_SIZE; ++i)
//    for (i=0; i<TTABLE_SIZE; ++i) if (transform_info[i].name != NULL)
     {
        if (transform_info[i].when & TRANSFORM_R)
           read_transforms |= transform_info[i].transform;
 
  if (transform_info[i].when & TRANSFORM_W)
          write_transforms |= transform_info[i].transform;
  }
 
   
    rw_transforms = read_transforms & write_transforms;
 }