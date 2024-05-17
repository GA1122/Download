void imap_free_header_data(struct ImapHeaderData **data)
{
  if (!data || !*data)
    return;

   
  FREE(&((*data)->flags_system));
  FREE(&((*data)->flags_remote));
  FREE(data);
}
