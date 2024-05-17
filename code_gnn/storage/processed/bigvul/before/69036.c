char *FLTGetSpatialComparisonCommonExpression(FilterEncodingNode *psNode, layerObj *lp)
{
  char *pszExpression = NULL;
  shapeObj *psQueryShape = NULL;
  double dfDistance = -1;
  int nUnit = -1, nLayerUnit = -1;
  char *pszWktText = NULL;
  char szBuffer[256];
  char *pszTmp=NULL;
  projectionObj sProjTmp;
  rectObj sQueryRect;
  shapeObj *psTmpShape=NULL;
  int bBBoxQuery = 0;
  int bAlreadyReprojected = 0;

  if (psNode == NULL || lp == NULL)
    return NULL;

  if (psNode->eType != FILTER_NODE_TYPE_SPATIAL)
    return NULL;

   
  if (FLTIsBBoxFilter(psNode)) {
    char szPolygon[512];
    FLTGetBBOX(psNode, &sQueryRect);

    snprintf(szPolygon, sizeof(szPolygon),
             "POLYGON((%.18f %.18f,%.18f %.18f,%.18f %.18f,%.18f %.18f,%.18f %.18f))",
             sQueryRect.minx, sQueryRect.miny,
             sQueryRect.minx, sQueryRect.maxy,
             sQueryRect.maxx, sQueryRect.maxy,
             sQueryRect.maxx, sQueryRect.miny,
             sQueryRect.minx, sQueryRect.miny);

    psTmpShape = msShapeFromWKT(szPolygon);

     
    if (fabs(sQueryRect.minx - -180.0) < 1e-5 &&
        fabs(sQueryRect.miny - -90.0) < 1e-5 &&
        fabs(sQueryRect.maxx - 180.0) < 1e-5 &&
        fabs(sQueryRect.maxy - 90.0) < 1e-5)
    {
      if (lp->projection.numargs > 0) {
        if (psNode->pszSRS)
          msInitProjection(&sProjTmp);
        if (psNode->pszSRS) {
           
          if (msLoadProjectionString(&sProjTmp, psNode->pszSRS) == 0) {
            msProjectRect(&sProjTmp, &lp->projection, &sQueryRect);
          }
        } else if (lp->map->projection.numargs > 0)
          msProjectRect(&lp->map->projection, &lp->projection, &sQueryRect);
        if (psNode->pszSRS)
          msFreeProjection(&sProjTmp);
      }
      if (sQueryRect.minx <= -1e14) {
        msFreeShape(psTmpShape);
        msFree(psTmpShape);
        psTmpShape = (shapeObj*) msSmallMalloc(sizeof(shapeObj));
        msInitShape(psTmpShape);
        msRectToPolygon(sQueryRect, psTmpShape);
        bAlreadyReprojected = 1;
      }
    }

    bBBoxQuery = 1;
  } else {
     

     

    psQueryShape = FLTGetShape(psNode, &dfDistance, &nUnit);

    if ((strcasecmp(psNode->pszValue, "DWithin") == 0 || strcasecmp(psNode->pszValue, "Beyond") == 0 ) && dfDistance > 0) {
      nLayerUnit = lp->units;
      if(nLayerUnit == -1) nLayerUnit = GetMapserverUnitUsingProj(&lp->projection);
      if(nLayerUnit == -1) nLayerUnit = lp->map->units;
      if(nLayerUnit == -1) nLayerUnit = GetMapserverUnitUsingProj(&lp->map->projection);

      if (nUnit >= 0 && nUnit != nLayerUnit)
        dfDistance *= msInchesPerUnit(nUnit,0)/msInchesPerUnit(nLayerUnit,0);  
    }

    psTmpShape = psQueryShape;
  }

  if (psTmpShape) {

     
    if (!bAlreadyReprojected && lp->projection.numargs > 0) {
      if (psNode->pszSRS)
        msInitProjection(&sProjTmp);
      if (psNode->pszSRS) {
         
        if (msLoadProjectionString(&sProjTmp, psNode->pszSRS) == 0) {
          msProjectShape(&sProjTmp, &lp->projection, psTmpShape);
        }
      } else if (lp->map->projection.numargs > 0)
        msProjectShape(&lp->map->projection, &lp->projection, psTmpShape);
      if (psNode->pszSRS)
        msFreeProjection(&sProjTmp);
    }

     
    if (bBBoxQuery) {
      sprintf(szBuffer, "%s", "intersects");
    } else {
      if (strncasecmp(psNode->pszValue, "intersect", 9) == 0)
        sprintf(szBuffer, "%s", "intersects");
      else {
        pszTmp = msStrdup(psNode->pszValue);
        msStringToLower(pszTmp);
        sprintf(szBuffer, "%s", pszTmp);
        msFree(pszTmp);
      }
    }
    pszExpression = msStringConcatenate(pszExpression, szBuffer);
    pszExpression = msStringConcatenate(pszExpression, "(");

     
    sprintf(szBuffer, "%s", "[shape]");
    pszExpression = msStringConcatenate(pszExpression, szBuffer);
    pszExpression = msStringConcatenate(pszExpression, ",");

     
    pszWktText = msGEOSShapeToWKT(psTmpShape);
    sprintf(szBuffer, "%s", "fromText('");
    pszExpression = msStringConcatenate(pszExpression, szBuffer);
    pszExpression = msStringConcatenate(pszExpression, pszWktText);
    sprintf(szBuffer, "%s", "')");
    pszExpression = msStringConcatenate(pszExpression, szBuffer);
    msGEOSFreeWKT(pszWktText);

     
    if ((strcasecmp(psNode->pszValue, "DWithin") == 0 || strcasecmp(psNode->pszValue, "Beyond") == 0)) {
      sprintf(szBuffer, ",%g", dfDistance);
      pszExpression = msStringConcatenate(pszExpression, szBuffer);      
    }

     
    pszExpression = msStringConcatenate(pszExpression, ") = TRUE");
  }

   
  if (bBBoxQuery) {
    msFreeShape(psTmpShape);
    msFree(psTmpShape);
  }

  return pszExpression;
}
