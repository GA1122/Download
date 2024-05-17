CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON * const item)
{
    if ((parent == NULL) || (item == NULL))
    {
        return NULL;
    }

    if (item->prev != NULL)
    {
         
        item->prev->next = item->next;
    }
    if (item->next != NULL)
    {
         
        item->next->prev = item->prev;
    }

    if (item == parent->child)
    {
         
        parent->child = item->next;
    }
     
    item->prev = NULL;
    item->next = NULL;

    return item;
}
