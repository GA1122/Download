char *FLTGetBinaryComparisonCommonExpression(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char szTmp[1024];
  char *pszExpression = NULL, *pszTmpEscaped;
  int bString;
  int bDateTime;

  if (psFilterNode == NULL)
    return NULL;

   
   
   
   
  bString = 0;
  bDateTime = 0;
  if (psFilterNode->psRightNode->pszValue) {
    const char* pszType;
    snprintf(szTmp, sizeof(szTmp), "%s_type",  psFilterNode->psLeftNode->pszValue);
    pszType = msOWSLookupMetadata(&(lp->metadata), "OFG", szTmp);
    if (pszType!= NULL && (strcasecmp(pszType, "Character") == 0))
      bString = 1;
    else if (pszType!= NULL && (strcasecmp(pszType, "Date") == 0))
      bDateTime = 1;
    else if (FLTIsNumeric(psFilterNode->psRightNode->pszValue) == MS_FALSE)
      bString = 1;
  }

   
   
  if (psFilterNode->psRightNode->pszValue == NULL || strcasecmp(psFilterNode->pszValue, "PropertyIsLike") == 0)
    bString = 1;

   
  if (bString)
    sprintf(szTmp, "%s", "(\"[");
  else
    sprintf(szTmp,  "%s","([");
  pszExpression = msStringConcatenate(pszExpression, szTmp);
  pszExpression = msStringConcatenate(pszExpression, psFilterNode->psLeftNode->pszValue);
  
  if (bString)
    sprintf(szTmp,  "%s","]\" ");
  else
    sprintf(szTmp,  "%s", "] ");
  pszExpression = msStringConcatenate(pszExpression, szTmp);

  if (strcasecmp(psFilterNode->pszValue, "PropertyIsEqualTo") == 0) {
     
    if (psFilterNode->psRightNode->pOther && (*(int *)psFilterNode->psRightNode->pOther) == 1)
      sprintf(szTmp,  "%s", "=*");
    else
      sprintf(szTmp,  "%s", "=");
  } else if (strcasecmp(psFilterNode->pszValue, "PropertyIsNotEqualTo") == 0)
    sprintf(szTmp,  "%s", "!=");
  else if (strcasecmp(psFilterNode->pszValue, "PropertyIsLessThan") == 0)
    sprintf(szTmp,  "%s", "<");
  else if (strcasecmp(psFilterNode->pszValue, "PropertyIsGreaterThan") == 0)
    sprintf(szTmp,  "%s", ">");
  else if (strcasecmp(psFilterNode->pszValue, "PropertyIsLessThanOrEqualTo") == 0)
    sprintf(szTmp,  "%s", "<=");
  else if (strcasecmp(psFilterNode->pszValue, "PropertyIsGreaterThanOrEqualTo") == 0)
    sprintf(szTmp,  "%s", ">=");
  else if (strcasecmp(psFilterNode->pszValue, "PropertyIsLike") == 0)
    sprintf(szTmp,  "%s", "~");

  pszExpression = msStringConcatenate(pszExpression, szTmp);
  pszExpression = msStringConcatenate(pszExpression, " ");

   
  if (bString) {
    sprintf(szTmp,  "%s", "\"");
    pszExpression = msStringConcatenate(pszExpression, szTmp);
  }
  else if (bDateTime) {
    sprintf(szTmp,  "%s", "`");
    pszExpression = msStringConcatenate(pszExpression, szTmp);
  }

  if (psFilterNode->psRightNode->pszValue) {
    pszTmpEscaped = msStringEscape(psFilterNode->psRightNode->pszValue);
    pszExpression = msStringConcatenate(pszExpression, pszTmpEscaped);
    if(pszTmpEscaped != psFilterNode->psRightNode->pszValue ) msFree(pszTmpEscaped);
  }

  if (bString) {
    sprintf(szTmp,  "%s", "\"");
    pszExpression = msStringConcatenate(pszExpression, szTmp);
  }
  else if (bDateTime) {
    sprintf(szTmp,  "%s", "`");
    pszExpression = msStringConcatenate(pszExpression, szTmp);
  }

  sprintf(szTmp,  "%s", ")");
  pszExpression = msStringConcatenate(pszExpression, szTmp);

  return pszExpression;
}