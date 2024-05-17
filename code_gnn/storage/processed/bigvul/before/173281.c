IDAT_list_size(struct IDAT_list *list, unsigned int length)
  
{
 if (list != NULL)
      length = list->length;

 return sizeof *list - sizeof list->lengths +
      length * sizeof list->lengths[0];
}
