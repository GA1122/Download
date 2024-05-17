xmlPointerListAddSize(xmlPointerListPtr list,
		       void *item,
		       int initialSize)
{
    if (list->items == NULL) {
	if (initialSize <= 0)
	    initialSize = 1;
	list->items = (void **) xmlMalloc(
	    initialSize * sizeof(void *));
	if (list->items == NULL) {
	    xmlXPathErrMemory(NULL,
		"xmlPointerListCreate: allocating item\n");
	    return(-1);
	}
	list->number = 0;
	list->size = initialSize;
    } else if (list->size <= list->number) {
	list->size *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->size * sizeof(void *));
	if (list->items == NULL) {
	    xmlXPathErrMemory(NULL,
		"xmlPointerListCreate: re-allocating item\n");
	    list->size = 0;
	    return(-1);
	}
    }
    list->items[list->number++] = item;
    return(0);
}
