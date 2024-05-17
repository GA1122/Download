static cJSON_bool add_item_to_array(cJSON *array, cJSON *item)
{
    cJSON *child = NULL;

    if ((item == NULL) || (array == NULL))
    {
        return false;
    }

    child = array->child;

    if (child == NULL)
    {
         
        array->child = item;
    }
    else
    {
         
        while (child->next)
        {
            child = child->next;
        }
        suffix_object(child, item);
    }

    return true;
}
