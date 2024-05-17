int FLTProcessPropertyIsNull(FilterEncodingNode *psFilterNode,
                            mapObj *map, int i)
{
    int status = MS_SUCCESS;

    if (psFilterNode->eType == FILTER_NODE_TYPE_COMPARISON &&
        psFilterNode->psLeftNode != NULL &&
        psFilterNode->psLeftNode->eType == FILTER_NODE_TYPE_PROPERTYNAME &&
        strcmp(psFilterNode->pszValue, "PropertyIsNull") == 0 &&
        !FLTIsGMLDefaultProperty(psFilterNode->psLeftNode->pszValue) )
    {
        layerObj* lp;
        int layerWasOpened;

        lp = GET_LAYER(map, i);
        layerWasOpened = msLayerIsOpen(lp);

         
        if( (lp->connectiontype == MS_POSTGIS ||
             (lp->connectiontype == MS_OGR && msOGRIsSpatialite(lp))) &&
            strcmp(psFilterNode->pszValue, "PropertyIsNull") == 0 )
        {
            msFree(psFilterNode->pszValue);
            psFilterNode->pszValue = msStrdup("PropertyIsEqualTo");
            psFilterNode->psRightNode = FLTCreateBinaryCompFilterEncodingNode();
            psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_LITERAL;
            psFilterNode->psRightNode->pszValue = msStrdup("_MAPSERVER_NULL_");
        }

        if (!layerWasOpened)  
          msLayerClose(lp);
    }

    if (psFilterNode->psLeftNode)
    {
      status = FLTProcessPropertyIsNull(psFilterNode->psLeftNode, map, i);
      if( status == MS_SUCCESS )
      {
        if (psFilterNode->psRightNode)
            status = FLTProcessPropertyIsNull(psFilterNode->psRightNode, map, i);
      }
    }
    return status;
}