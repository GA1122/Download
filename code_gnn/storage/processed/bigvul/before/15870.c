_eina_unicode_to_hex(Eina_Unicode u)
{
   if (u >= '0' && u <= '9')
     return u - '0';
   if (u >= 'a' && u <= 'f')
     return 10 + u - 'a';
   if (u >= 'A' && u <= 'F')
     return 10 + u - 'A';
   return -1;
}
