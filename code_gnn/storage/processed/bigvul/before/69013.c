int FLTLayerApplyPlainFilterToLayer(FilterEncodingNode *psNode, mapObj *map,
                                    int iLayerIndex)
{
  char *pszExpression  =NULL;
  int status =MS_FALSE;
  layerObj* lp = GET_LAYER(map, iLayerIndex);

  pszExpression = FLTGetCommonExpression(psNode,  lp);
  if (pszExpression) {
    const char* pszUseDefaultExtent;
    FilterEncodingNode* psTopBBOX;
    rectObj rect = map->extent;

    pszUseDefaultExtent = msOWSLookupMetadata(&(lp->metadata), "F",
                                              "use_default_extent_for_getfeature");
    if( pszUseDefaultExtent && !CSLTestBoolean(pszUseDefaultExtent) &&
        lp->connectiontype == MS_OGR )
    {
        const rectObj rectInvalid = MS_INIT_INVALID_RECT;
        rect = rectInvalid;
    }

    psTopBBOX = FLTGetTopBBOX(psNode);
    if( psTopBBOX )
    {
      int can_remove_expression = MS_TRUE;
      const char* pszEPSG = FLTGetBBOX(psNode, &rect);
      if(pszEPSG && map->projection.numargs > 0) {
        projectionObj sProjTmp;
        msInitProjection(&sProjTmp);
         
        if (msLoadProjectionString(&sProjTmp, pszEPSG) == 0) {
          rectObj oldRect = rect;
          msProjectRect(&sProjTmp, &map->projection, &rect);
           
          if( rect.minx != oldRect.minx ||
              rect.miny != oldRect.miny ||
              rect.maxx != oldRect.maxx ||
              rect.maxy != oldRect.maxy )
          {
            can_remove_expression = MS_FALSE;
          }
        }
        msFreeProjection(&sProjTmp);
      }

       
       
      if( psTopBBOX == psNode && can_remove_expression )
      {
        msFree(pszExpression);
        pszExpression = NULL;
      }
    }

    if(map->debug == MS_DEBUGLEVEL_VVV)
    {
      if( pszExpression )
        msDebug("FLTLayerApplyPlainFilterToLayer(): %s, rect=%.15g,%.15g,%.15g,%.15g\n", pszExpression, rect.minx, rect.miny, rect.maxx, rect.maxy);
      else
        msDebug("FLTLayerApplyPlainFilterToLayer(): rect=%.15g,%.15g,%.15g,%.15g\n", rect.minx, rect.miny, rect.maxx, rect.maxy);
    }

    status = FLTApplyFilterToLayerCommonExpressionWithRect(map, iLayerIndex,
                                                           pszExpression, rect);
    msFree(pszExpression);
  }

  return status;
}