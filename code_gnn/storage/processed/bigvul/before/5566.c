tristring_hash (gconstpointer key)
{
  const char *p = key;
  guint h = *p;

  if (h)
    {
      for (p += 1; *p != '\0'; p++)
        h = (h << 5) - h + *p;
    }

   
  for (p += 1; *p != '\0'; p++)
    h = (h << 5) - h + *p;

   
  for (p += 1; *p != '\0'; p++)
    h = (h << 5) - h + *p;
  
  return h;
}
