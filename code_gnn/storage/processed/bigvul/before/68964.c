int FLTApplySimpleSQLFilter(FilterEncodingNode *psNode, mapObj *map, int iLayerIndex)
{
  layerObj *lp = NULL;
  char *szExpression = NULL;
  rectObj sQueryRect = map->extent;
  const char *szEPSG = NULL;
  projectionObj sProjTmp;
  char *pszBuffer = NULL;
  int bConcatWhere = 0;
  int bHasAWhere =0;
  char *pszTmp = NULL, *pszTmp2 = NULL;
  char *tmpfilename = NULL;
  const char* pszTimeField = NULL;
  const char* pszTimeValue = NULL;

  lp = (GET_LAYER(map, iLayerIndex));

   
  szEPSG = FLTGetBBOX(psNode, &sQueryRect);
  if(szEPSG && map->projection.numargs > 0) {
    msInitProjection(&sProjTmp);
     
    if (msLoadProjectionString(&sProjTmp, szEPSG) == 0) {
      msProjectRect(&sProjTmp, &map->projection, &sQueryRect);
    }
    msFreeProjection(&sProjTmp);
  }
  
  if( lp->connectiontype == MS_OGR ) {
    pszTimeValue = FLTGetDuring(psNode, &pszTimeField);
  }

   
  if (!lp->template) lp->template = msStrdup("ttt.html");

   
  if (lp->numclasses == 0) {
    if (msGrowLayerClasses(lp) == NULL)
      return MS_FAILURE;
    initClass(lp->class[0]);
  }

  bConcatWhere = 0;
  bHasAWhere = 0;
  if (lp->connectiontype == MS_POSTGIS || lp->connectiontype ==  MS_ORACLESPATIAL ||
      lp->connectiontype == MS_PLUGIN) {
    szExpression = FLTGetSQLExpression(psNode, lp);
    if (szExpression) {
      pszTmp = msStrdup("(");
      pszTmp = msStringConcatenate(pszTmp, szExpression);
      pszTmp = msStringConcatenate(pszTmp, ")");
      msFree(szExpression);
      szExpression = pszTmp;
    }
  }
   
  else if (lp->connectiontype == MS_OGR) {
    if (lp->filter.type != MS_EXPRESSION) {
      szExpression = FLTGetSQLExpression(psNode, lp);
      bConcatWhere = 1;
    } else {
      if (lp->filter.string && EQUALN(lp->filter.string,"WHERE ",6)) {
        szExpression = FLTGetSQLExpression(psNode, lp);
        bHasAWhere = 1;
        bConcatWhere =1;
      } else {
        szExpression = FLTGetCommonExpression(psNode, lp);
      }
    }
  } else {
    szExpression = FLTGetCommonExpression(psNode, lp);

  }

  if (szExpression) {
    if (bConcatWhere)
      pszBuffer = msStringConcatenate(pszBuffer, "WHERE ");

     
    if (lp->filter.string && lp->filter.type == MS_EXPRESSION) {
      pszBuffer = msStringConcatenate(pszBuffer, "((");
      if (bHasAWhere)
        pszBuffer = msStringConcatenate(pszBuffer, lp->filter.string+6);
      else
        pszBuffer = msStringConcatenate(pszBuffer, lp->filter.string);
      pszBuffer = msStringConcatenate(pszBuffer, ") and ");
    } else if (lp->filter.string)
      msFreeExpression(&lp->filter);

    pszBuffer = msStringConcatenate(pszBuffer, szExpression);

    if(lp->filter.string && lp->filter.type == MS_EXPRESSION)
      pszBuffer = msStringConcatenate(pszBuffer, ")");

    msLoadExpressionString(&lp->filter, pszBuffer);
    free(szExpression);
  }
  
  if (pszTimeField && pszTimeValue)
      msLayerSetTimeFilter(lp, pszTimeValue, pszTimeField);

  if (pszBuffer)
    free(pszBuffer);

  map->query.type = MS_QUERY_BY_RECT;
  map->query.mode = MS_QUERY_MULTIPLE;
  map->query.layer = lp->index;
  map->query.rect = sQueryRect;

  if(map->debug == MS_DEBUGLEVEL_VVV) {
    tmpfilename = msTmpFile(map, map->mappath, NULL, "_filter.map");
    if (tmpfilename == NULL) {
      tmpfilename = msTmpFile(map, NULL, NULL, "_filter.map" );
    }
    if (tmpfilename) {
      msSaveMap(map,tmpfilename);
      msDebug("FLTApplySimpleSQLFilter(): Map file after Filter was applied %s\n", tmpfilename);
      msFree(tmpfilename);
    }
  }

   

  if (lp->connectiontype ==  MS_ORACLESPATIAL && FLTIsSimpleFilterNoSpatial(psNode)) {
    if (strcasestr(lp->data, "USING") == 0)
      lp->data = msStringConcatenate(lp->data, " USING NONE");
    else if (strcasestr(lp->data, "NONE") == 0) {
       
      if (strcasestr(lp->data, "FILTER"))
        lp->data = msCaseReplaceSubstring(lp->data, "FILTER", "NONE");
      else if (strcasestr(lp->data, "GEOMRELATE"))
        lp->data = msCaseReplaceSubstring(lp->data, "GEOMRELATE", "NONE");
      else if (strcasestr(lp->data, "RELATE"))
        lp->data = msCaseReplaceSubstring(lp->data, "RELATE", "NONE");
      else if (strcasestr(lp->data, "VERSION")) {
         
        pszTmp = (char *)strcasestr(lp->data, "VERSION");
        pszTmp2 = msStringConcatenate(pszTmp2, " NONE ");
        pszTmp2 = msStringConcatenate(pszTmp2, pszTmp);

        lp->data = msCaseReplaceSubstring(lp->data, pszTmp, pszTmp2);

        msFree(pszTmp2);

      } else if (strcasestr(lp->data, "SRID")) {
        lp->data = msStringConcatenate(lp->data, " NONE");
      }
    }
  }

  return msQueryByRect(map);

   
}
