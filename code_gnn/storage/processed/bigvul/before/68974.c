char *FLTGetBinaryComparisonExpresssion(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  const size_t bufferSize = 1024;
  char szBuffer[1024];
  int bString=0;
  char szTmp[256];

  szBuffer[0] = '\0';
  if (!psFilterNode || !FLTIsBinaryComparisonFilterType(psFilterNode->pszValue))
    return NULL;

   
   
   
   
  bString = 0;
  if (psFilterNode->psRightNode->pszValue) {
    const char* pszOFGType;
    snprintf(szTmp, sizeof(szTmp), "%s_type",  psFilterNode->psLeftNode->pszValue);
    pszOFGType = msOWSLookupMetadata(&(lp->metadata), "OFG", szTmp);
    if (pszOFGType!= NULL && strcasecmp(pszOFGType, "Character") == 0)
      bString = 1;
    else if (FLTIsNumeric(psFilterNode->psRightNode->pszValue) == MS_FALSE)
      bString = 1;
  }

   
  if (psFilterNode->psRightNode->pszValue == NULL)
    bString = 1;


  if (bString)
    strlcat(szBuffer, " (\"[", bufferSize);
  else
    strlcat(szBuffer, " ([", bufferSize);
   

  strlcat(szBuffer, psFilterNode->psLeftNode->pszValue, bufferSize);
  if (bString)
    strlcat(szBuffer, "]\" ", bufferSize);
  else
    strlcat(szBuffer, "] ", bufferSize);


   
  if (strcasecmp(psFilterNode->pszValue,
                 "PropertyIsEqualTo") == 0) {
     
    if (psFilterNode->psRightNode->pOther &&
        (*(int *)psFilterNode->psRightNode->pOther) == 1) {
      strlcat(szBuffer, "IEQ", bufferSize);
    } else
      strlcat(szBuffer, "=", bufferSize);
  } else if (strcasecmp(psFilterNode->pszValue,
                        "PropertyIsNotEqualTo") == 0)
    strlcat(szBuffer, "!=", bufferSize);
  else if (strcasecmp(psFilterNode->pszValue,
                      "PropertyIsLessThan") == 0)
    strlcat(szBuffer, "<", bufferSize);
  else if (strcasecmp(psFilterNode->pszValue,
                      "PropertyIsGreaterThan") == 0)
    strlcat(szBuffer, ">", bufferSize);
  else if (strcasecmp(psFilterNode->pszValue,
                      "PropertyIsLessThanOrEqualTo") == 0)
    strlcat(szBuffer, "<=", bufferSize);
  else if (strcasecmp(psFilterNode->pszValue,
                      "PropertyIsGreaterThanOrEqualTo") == 0)
    strlcat(szBuffer, ">=", bufferSize);

  strlcat(szBuffer, " ", bufferSize);

   
  if (bString)
    strlcat(szBuffer, "\"", bufferSize);

  if (psFilterNode->psRightNode->pszValue)
    strlcat(szBuffer, psFilterNode->psRightNode->pszValue, bufferSize);

  if (bString)
    strlcat(szBuffer, "\"", bufferSize);

  strlcat(szBuffer, ") ", bufferSize);

  return msStrdup(szBuffer);
}