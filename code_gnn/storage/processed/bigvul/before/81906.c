int wc_ecc_is_valid_idx(int n)
{
   int x;

   for (x = 0; ecc_sets[x].size != 0; x++)
       ;
    
   if ((n >= ECC_CUSTOM_IDX) && (n < x)) {
      return 1;
   }

   return 0;
}