int FLTCheckInvalidProperty(FilterEncodingNode *psFilterNode,
                            mapObj *map, int i)
{
    int status = MS_SUCCESS;

    if (psFilterNode->eType ==  FILTER_NODE_TYPE_COMPARISON &&
        psFilterNode->psLeftNode != NULL &&
        psFilterNode->psLeftNode->eType == FILTER_NODE_TYPE_PROPERTYNAME)
    {
        layerObj* lp;
        int layerWasOpened;
        int bFound = MS_FALSE;

        if ((strcmp(psFilterNode->pszValue, "PropertyIsNull") == 0 ||
             strcmp(psFilterNode->pszValue, "PropertyIsNil") == 0) && 
             FLTIsGMLDefaultProperty(psFilterNode->psLeftNode->pszValue) )
        {
            return MS_SUCCESS;
        }

        lp = GET_LAYER(map, i);
        layerWasOpened = msLayerIsOpen(lp);
        if ((layerWasOpened || msLayerOpen(lp) == MS_SUCCESS) 
             && msLayerGetItems(lp) == MS_SUCCESS) {
          int i;
          gmlItemListObj* items = msGMLGetItems(lp, "G");
          for(i=0; i<items->numitems; i++) {
            if (!items->items[i].name || strlen(items->items[i].name) <= 0 ||
                !items->items[i].visible)
              continue;
            if (strcasecmp(items->items[i].name, psFilterNode->psLeftNode->pszValue) == 0) {
                bFound = MS_TRUE;
                break;
            }
          }
          msGMLFreeItems(items);
        }

        if (!layerWasOpened)  
          msLayerClose(lp);

        if( !bFound )
        {
            msSetError(MS_MISCERR, "Property '%s' is unknown.",
                       "FLTCheckInvalidProperty()", psFilterNode->psLeftNode->pszValue);
            return MS_FAILURE;
        }
    }

    if (psFilterNode->psLeftNode)
    {
      status = FLTCheckInvalidProperty(psFilterNode->psLeftNode, map, i);
      if( status == MS_SUCCESS )
      {
        if (psFilterNode->psRightNode)
            status = FLTCheckInvalidProperty(psFilterNode->psRightNode, map, i);
      }
    }
    return status;
}