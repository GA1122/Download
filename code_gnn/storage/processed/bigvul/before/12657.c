expand_getlistele(int field, uschar * list)
{
uschar * tlist= list;
int sep= 0;
uschar dummy;

if(field<0)
{
  for(field++; string_nextinlist(&tlist, &sep, &dummy, 1); ) field++;
  sep= 0;
}
if(field==0) return NULL;
while(--field>0 && (string_nextinlist(&list, &sep, &dummy, 1))) ;
return string_nextinlist(&list, &sep, NULL, 0);
}
