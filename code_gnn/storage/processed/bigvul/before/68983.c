char *FLTGetIsLikeComparisonSQLExpression(FilterEncodingNode *psFilterNode,
    layerObj *lp)
{
  const size_t bufferSize = 1024;
  char szBuffer[1024];
  char *pszValue = NULL;

  const char *pszWild = NULL;
  const char *pszSingle = NULL;
  const char *pszEscape = NULL;
  char szTmp[4];

  int nLength=0, i=0, j=0;
  int  bCaseInsensitive = 0;

  char *pszEscapedStr = NULL;
  FEPropertyIsLike* propIsLike;

  if (!psFilterNode || !psFilterNode->pOther || !psFilterNode->psLeftNode ||
      !psFilterNode->psRightNode || !psFilterNode->psRightNode->pszValue)
    return NULL;

  propIsLike = (FEPropertyIsLike *)psFilterNode->pOther;
  pszWild = propIsLike->pszWildCard;
  pszSingle = propIsLike->pszSingleChar;
  pszEscape = propIsLike->pszEscapeChar;
  bCaseInsensitive = propIsLike->bCaseInsensitive;

  if (!pszWild || strlen(pszWild) == 0 ||
      !pszSingle || strlen(pszSingle) == 0 ||
      !pszEscape || strlen(pszEscape) == 0)
    return NULL;

  if (pszEscape[0] == '\'') {
     
     
     
    msSetError(MS_MISCERR, "Single quote character is not allowed as an escaping character.",
               "FLTGetIsLikeComparisonSQLExpression()");
    return NULL;
  }


  szBuffer[0] = '\0';
   
  strlcat(szBuffer, " (", bufferSize);

   
  pszEscapedStr = msLayerEscapePropertyName(lp, psFilterNode->psLeftNode->pszValue);

  strlcat(szBuffer, pszEscapedStr, bufferSize);
  msFree(pszEscapedStr);
  pszEscapedStr = NULL;

  if (lp->connectiontype == MS_POSTGIS) {
    if (bCaseInsensitive == 1)
      strlcat(szBuffer, "::text ilike '", bufferSize);
    else
      strlcat(szBuffer, "::text like '", bufferSize);
  } else
    strlcat(szBuffer, " like '", bufferSize);

  pszValue = psFilterNode->psRightNode->pszValue;
  nLength = strlen(pszValue);

  pszEscapedStr = (char*) msSmallMalloc( 3 * nLength + 1);

  for (i=0, j=0; i<nLength; i++) {
    char c = pszValue[i];
    if (c != pszWild[0] &&
        c != pszSingle[0] &&
        c != pszEscape[0]) {
      if (c == '\'') {
        pszEscapedStr[j++] = '\'';
        pszEscapedStr[j++] = '\'';
      } else if (c == '\\') {
        pszEscapedStr[j++] = '\\';
        pszEscapedStr[j++] = '\\';
      } else
        pszEscapedStr[j++] = c;
    } else if  (c == pszSingle[0]) {
      pszEscapedStr[j++] = '_';
    } else if  (c == pszEscape[0]) {
      pszEscapedStr[j++] = pszEscape[0];
      if (i+1<nLength) {
        char nextC = pszValue[i+1];
        i++;
        if (nextC == '\'') {
          pszEscapedStr[j++] = '\'';
          pszEscapedStr[j++] = '\'';
        } else
          pszEscapedStr[j++] = nextC;
      }
    } else if (c == pszWild[0]) {
      pszEscapedStr[j++] = '%';
    }
  }
  pszEscapedStr[j++] = 0;
  strlcat(szBuffer, pszEscapedStr, bufferSize);
  msFree(pszEscapedStr);

  strlcat(szBuffer, "'", bufferSize);
  if (lp->connectiontype != MS_OGR) {
    if (lp->connectiontype == MS_POSTGIS && pszEscape[0] == '\\')
        strlcat(szBuffer, " escape E'", bufferSize);
    else
        strlcat(szBuffer, " escape '", bufferSize);
    szTmp[0] = pszEscape[0];
    if (pszEscape[0] == '\\') {
      szTmp[1] = '\\';
      szTmp[2] = '\'';
      szTmp[3] = '\0';
    } else {
      szTmp[1] = '\'';
      szTmp[2] = '\0';
    }

    strlcat(szBuffer,  szTmp, bufferSize);
  }
  strlcat(szBuffer,  ") ", bufferSize);

  return msStrdup(szBuffer);
}