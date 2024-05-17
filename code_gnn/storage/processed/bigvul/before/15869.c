_csi_arg_get(Eina_Unicode **ptr)
{
   Eina_Unicode *b = *ptr;
   int sum = 0;

   while ((*b) && (*b < '0' || *b > '9')) b++;
   if (!*b)
     {
        *ptr = NULL;
        return 0;
     }
   while ((*b >= '0') && (*b <= '9'))
     {
        sum *= 10;
        sum += *b - '0';
        b++;
     }
   *ptr = b;
   return sum;
}
