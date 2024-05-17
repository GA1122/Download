xmlPointerListFree(xmlPointerListPtr list)
{
    if (list == NULL)
	return;
    if (list->items != NULL)
	xmlFree(list->items);
    xmlFree(list);
}
