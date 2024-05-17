get_word(char *s,
         char *d)
{
   char *p1, *p2;

   p1 = s;
   p2 = d;
   while (*p1)
     {
        if (p2 == d)
          {
             if (isspace(*p1))
               {
                  p1++;
                  continue;
               }
          }
        if (isspace(*p1)) break;
        *p2 = *p1;
        p1++;
        p2++;
     }
   *p2 = 0;
   return p1;
}
