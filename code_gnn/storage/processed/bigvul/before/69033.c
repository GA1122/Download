char *FLTGetFeatureIdCommonExpression(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char *pszExpression = NULL;
  int nTokens = 0, i=0, bString=0;
  char **tokens = NULL;
  const char *pszAttribute=NULL;

#if defined(USE_WMS_SVR) || defined(USE_WFS_SVR) || defined(USE_WCS_SVR) || defined(USE_SOS_SVR)
  if (psFilterNode->pszValue) {
    pszAttribute = msOWSLookupMetadata(&(lp->metadata), "OFG", "featureid");
    if (pszAttribute) {
      tokens = msStringSplit(psFilterNode->pszValue,',', &nTokens);
      if (tokens && nTokens > 0) {
        for (i=0; i<nTokens; i++) {
          char *pszTmp = NULL;
          int bufferSize = 0;
          const char* pszId = tokens[i];
          const char* pszDot = strchr(pszId, '.');
          if( pszDot )
            pszId = pszDot + 1;

          if (i == 0) {
            if(FLTIsNumeric(pszId) == MS_FALSE)
              bString = 1;
          }

          if (bString) {
            bufferSize = 11+strlen(pszId)+strlen(pszAttribute)+1;
            pszTmp = (char *)msSmallMalloc(bufferSize);
            snprintf(pszTmp, bufferSize, "(\"[%s]\" ==\"%s\")" , pszAttribute, pszId);
          } else {
            bufferSize = 8+strlen(pszId)+strlen(pszAttribute)+1;
            pszTmp = (char *)msSmallMalloc(bufferSize);
            snprintf(pszTmp, bufferSize, "([%s] == %s)" , pszAttribute, pszId);
          }

          if (pszExpression != NULL)
            pszExpression = msStringConcatenate(pszExpression, " OR ");
          else
            pszExpression = msStringConcatenate(pszExpression, "(");
          pszExpression = msStringConcatenate(pszExpression, pszTmp);
          msFree(pszTmp);
        }

        msFreeCharArray(tokens, nTokens);
      }
    }

     
    if (pszExpression)
      pszExpression = msStringConcatenate(pszExpression, ")");
  }
#endif

  return pszExpression;
}