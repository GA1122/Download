rend_get_service_list_mutable(smartlist_t* substitute_service_list)
{
  if (substitute_service_list) {
    return substitute_service_list;
  }

   

  if (BUG(!rend_service_list)) {
     
    return NULL;
  }

  return rend_service_list;
}
