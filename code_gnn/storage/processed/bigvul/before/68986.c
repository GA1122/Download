char *FLTGetMapserverExpression(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char *pszExpression = NULL;
  const char *pszAttribute = NULL;
  char szTmp[256];
  char **tokens = NULL;
  int nTokens = 0, i=0,bString=0;

  if (!psFilterNode)
    return NULL;

  if (psFilterNode->eType == FILTER_NODE_TYPE_COMPARISON) {
    if ( psFilterNode->psLeftNode && psFilterNode->psRightNode) {
      if (FLTIsBinaryComparisonFilterType(psFilterNode->pszValue)) {
        pszExpression = FLTGetBinaryComparisonExpresssion(psFilterNode, lp);
      } else if (strcasecmp(psFilterNode->pszValue,
                            "PropertyIsBetween") == 0) {
        pszExpression = FLTGetIsBetweenComparisonExpresssion(psFilterNode, lp);
      } else if (strcasecmp(psFilterNode->pszValue,
                            "PropertyIsLike") == 0) {
        pszExpression = FLTGetIsLikeComparisonExpression(psFilterNode);
      }
    }
  } else if (psFilterNode->eType == FILTER_NODE_TYPE_LOGICAL) {
    if (strcasecmp(psFilterNode->pszValue, "AND") == 0 ||
        strcasecmp(psFilterNode->pszValue, "OR") == 0) {
      pszExpression = FLTGetLogicalComparisonExpresssion(psFilterNode, lp);
    } else if (strcasecmp(psFilterNode->pszValue, "NOT") == 0) {
      pszExpression = FLTGetLogicalComparisonExpresssion(psFilterNode, lp);
    }
  } else if (psFilterNode->eType == FILTER_NODE_TYPE_SPATIAL) {
     
  } else if (psFilterNode->eType == FILTER_NODE_TYPE_FEATUREID) {
#if defined(USE_WMS_SVR) || defined (USE_WFS_SVR) || defined (USE_WCS_SVR) || defined(USE_SOS_SVR)
    if (psFilterNode->pszValue) {
      pszAttribute = msOWSLookupMetadata(&(lp->metadata), "OFG", "featureid");
      if (pszAttribute) {
        tokens = msStringSplit(psFilterNode->pszValue,',', &nTokens);
        if (tokens && nTokens > 0) {
          for (i=0; i<nTokens; i++) {
            const char* pszId = tokens[i];
            const char* pszDot = strchr(pszId, '.');
            if( pszDot )
                pszId = pszDot + 1;
            if (i == 0) {
              if(FLTIsNumeric(pszId) == MS_FALSE)
                bString = 1;
            }
            if (bString)
              snprintf(szTmp, sizeof(szTmp), "('[%s]' = '%s')" , pszAttribute, pszId);
            else
              snprintf(szTmp, sizeof(szTmp), "([%s] = %s)" , pszAttribute, pszId);

            if (pszExpression != NULL)
              pszExpression = msStringConcatenate(pszExpression, " OR ");
            else
              pszExpression = msStringConcatenate(pszExpression, "(");
            pszExpression = msStringConcatenate(pszExpression, szTmp);
          }

          msFreeCharArray(tokens, nTokens);
        }
      }
       
      if (pszExpression)
        pszExpression = msStringConcatenate(pszExpression, ")");
    }
#else
    msSetError(MS_MISCERR, "OWS support is not available.",
               "FLTGetMapserverExpression()");
    return(MS_FAILURE);
#endif

  }
  return pszExpression;
}