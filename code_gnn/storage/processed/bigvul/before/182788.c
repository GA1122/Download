 static int lookup1_values(int entries, int dim)
 {
     int r = (int) floor(exp((float) log((float) entries) / dim));
     if ((int) floor(pow((float) r+1, dim)) <= entries)    
        ++r;                                               
   assert(pow((float) r+1, dim) > entries);
   assert((int) floor(pow((float) r, dim)) <= entries);  
//    if (pow((float) r+1, dim) <= entries)
//       return -1;
//    if ((int) floor(pow((float) r, dim)) > entries)
//       return -1;
     return r;
  }