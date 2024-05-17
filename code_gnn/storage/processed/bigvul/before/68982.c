char *FLTGetIsBetweenComparisonSQLExpresssion(FilterEncodingNode *psFilterNode,
    layerObj *lp)
{
  const size_t bufferSize = 1024;
  char szBuffer[1024];
  char **aszBounds = NULL;
  int nBounds = 0;
  int bString=0;
  char szTmp[256];
  char* pszEscapedStr;

  szBuffer[0] = '\0';
  if (!psFilterNode ||
      !(strcasecmp(psFilterNode->pszValue, "PropertyIsBetween") == 0))
    return NULL;

  if (!psFilterNode->psLeftNode || !psFilterNode->psRightNode )
    return NULL;

   
   
   
  aszBounds = msStringSplit(psFilterNode->psRightNode->pszValue, ';', &nBounds);
  if (nBounds != 2) {
    msFreeCharArray(aszBounds, nBounds);
    return NULL;
  }
   
   
   
   
  bString = 0;
  if (aszBounds[0]) {
    const char* pszOFGType;
    snprintf(szTmp, sizeof(szTmp), "%s_type",  psFilterNode->psLeftNode->pszValue);
    pszOFGType = msOWSLookupMetadata(&(lp->metadata), "OFG", szTmp);
    if (pszOFGType!= NULL && strcasecmp(pszOFGType, "Character") == 0)
      bString = 1;
    else if (FLTIsNumeric(aszBounds[0]) == MS_FALSE)
      bString = 1;
  }
  if (!bString) {
    if (aszBounds[1]) {
      if (FLTIsNumeric(aszBounds[1]) == MS_FALSE)
        bString = 1;
    }
  }


   
   
   
   
  strlcat(szBuffer, " (", bufferSize);

   
  pszEscapedStr = msLayerEscapePropertyName(lp, psFilterNode->psLeftNode->pszValue);

  strlcat(szBuffer, pszEscapedStr, bufferSize);
  msFree(pszEscapedStr);
  pszEscapedStr = NULL;

   
  strlcat(szBuffer, " BETWEEN ", bufferSize);

   
  if (bString)
    strlcat(szBuffer,"'", bufferSize);
  pszEscapedStr = msLayerEscapeSQLParam( lp, aszBounds[0]);
  strlcat(szBuffer, pszEscapedStr, bufferSize);
  msFree(pszEscapedStr);
  pszEscapedStr=NULL;

  if (bString)
    strlcat(szBuffer,"'", bufferSize);

  strlcat(szBuffer, " AND ", bufferSize);

   
  if (bString)
    strlcat(szBuffer, "'", bufferSize);
  pszEscapedStr = msLayerEscapeSQLParam( lp, aszBounds[1]);
  strlcat(szBuffer, pszEscapedStr, bufferSize);
  msFree(pszEscapedStr);
  pszEscapedStr=NULL;

  if (bString)
    strlcat(szBuffer,"'", bufferSize);

   
  strlcat(szBuffer, ")", bufferSize);

  msFreeCharArray(aszBounds, nBounds);

  return msStrdup(szBuffer);
}
