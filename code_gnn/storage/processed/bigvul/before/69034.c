char *FLTGetIsBetweenComparisonCommonExpresssion(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  const size_t bufferSize = 1024;
  char szBuffer[1024];
  char **aszBounds = NULL;
  int nBounds = 0;
  int bString=0;
  int bDateTime = 0;
  char *pszExpression=NULL, *pszTmpEscaped;

  if (!psFilterNode || !(strcasecmp(psFilterNode->pszValue, "PropertyIsBetween") == 0))
    return NULL;

  if (psFilterNode->psLeftNode == NULL || psFilterNode->psRightNode == NULL )
    return NULL;

   
   
   
  aszBounds = msStringSplit(psFilterNode->psRightNode->pszValue, ';', &nBounds);
  if (nBounds != 2) {
    msFreeCharArray(aszBounds, nBounds);
    return NULL;
  }

   
   
   
   
  bString = 0;
  if (aszBounds[0]) {
    const char* pszType;
    snprintf(szBuffer,  bufferSize, "%s_type",  psFilterNode->psLeftNode->pszValue);
    pszType = msOWSLookupMetadata(&(lp->metadata), "OFG", szBuffer);
    if (pszType != NULL && (strcasecmp(pszType, "Character") == 0))
      bString = 1;
    else if (pszType != NULL && (strcasecmp(pszType, "Date") == 0))
      bDateTime = 1;
    else if (FLTIsNumeric(aszBounds[0]) == MS_FALSE)
      bString = 1;
  }
  if (!bString && !bDateTime) {
    if (aszBounds[1]) {
      if (FLTIsNumeric(aszBounds[1]) == MS_FALSE)
        bString = 1;
    }
  }

   
   
   
   
  if (bString)
    sprintf(szBuffer, "%s", "(\"[");
  else
    sprintf(szBuffer, "%s", "([");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);
  
  pszExpression = msStringConcatenate(pszExpression, psFilterNode->psLeftNode->pszValue);

  if (bString)
    sprintf(szBuffer, "%s", "]\" ");
  else
    sprintf(szBuffer, "%s", "] ");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

  sprintf(szBuffer, "%s", " >= ");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

  if (bString) {
    pszExpression = msStringConcatenate(pszExpression, "\"");
  }
  else if (bDateTime) {
    pszExpression = msStringConcatenate(pszExpression, "`");
  }

  pszTmpEscaped = msStringEscape(aszBounds[0]);
  snprintf(szBuffer, bufferSize, "%s", pszTmpEscaped);
  if(pszTmpEscaped != aszBounds[0] ) msFree(pszTmpEscaped);
  pszExpression = msStringConcatenate(pszExpression, szBuffer);
  if (bString) {
    pszExpression = msStringConcatenate(pszExpression, "\"");
  }
  else if (bDateTime) {
    pszExpression = msStringConcatenate(pszExpression, "`");
  }

  sprintf(szBuffer, "%s", " AND ");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

  if (bString)
    sprintf(szBuffer, "%s", " \"[");
  else
    sprintf(szBuffer, "%s", " [");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

   
  pszExpression = msStringConcatenate(pszExpression, psFilterNode->psLeftNode->pszValue);

  if (bString)
    sprintf(szBuffer, "%s", "]\" ");
  else
    sprintf(szBuffer, "%s", "] ");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

  sprintf(szBuffer, "%s", " <= ");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);
  if (bString) {
    pszExpression = msStringConcatenate(pszExpression, "\"");
  }
  else if (bDateTime) {
    pszExpression = msStringConcatenate(pszExpression, "`");
  }
  pszTmpEscaped = msStringEscape(aszBounds[1]);
  snprintf(szBuffer, bufferSize, "%s", pszTmpEscaped);
  if (pszTmpEscaped != aszBounds[1]) msFree(pszTmpEscaped);
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

  if (bString) {
    pszExpression = msStringConcatenate(pszExpression, "\"");
  }
  else if (bDateTime) {
    pszExpression = msStringConcatenate(pszExpression, "`");
  }
  sprintf(szBuffer, "%s", ")");
  pszExpression = msStringConcatenate(pszExpression, szBuffer);

  msFreeCharArray(aszBounds, nBounds);

  return pszExpression;
}