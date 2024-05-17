compare_clients(cupsd_client_t *a,	 
                cupsd_client_t *b,	 
                void           *data)	 
{
  (void)data;

  if (a == b)
    return (0);
  else if (a < b)
    return (-1);
  else
    return (1);
}
