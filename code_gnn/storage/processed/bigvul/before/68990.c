char *FLTGetSQLExpression(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char *pszExpression = NULL;
  const char *pszAttribute = NULL;
  char szTmp[256];
  char **tokens = NULL;
  int nTokens = 0, i=0, bString=0;

  if (psFilterNode == NULL || lp == NULL)
    return NULL;

  if (psFilterNode->eType == FILTER_NODE_TYPE_COMPARISON) {
    if ( psFilterNode->psLeftNode && psFilterNode->psRightNode) {
      if (FLTIsBinaryComparisonFilterType(psFilterNode->pszValue)) {
        pszExpression =
          FLTGetBinaryComparisonSQLExpresssion(psFilterNode, lp);
      } else if (strcasecmp(psFilterNode->pszValue,
                            "PropertyIsBetween") == 0) {
        pszExpression =
          FLTGetIsBetweenComparisonSQLExpresssion(psFilterNode, lp);
      } else if (strcasecmp(psFilterNode->pszValue,
                            "PropertyIsLike") == 0) {
        pszExpression =
          FLTGetIsLikeComparisonSQLExpression(psFilterNode, lp);

      }
    }
  } else if (psFilterNode->eType == FILTER_NODE_TYPE_LOGICAL) {
    if (strcasecmp(psFilterNode->pszValue, "AND") == 0 ||
        strcasecmp(psFilterNode->pszValue, "OR") == 0) {
      pszExpression =
        FLTGetLogicalComparisonSQLExpresssion(psFilterNode, lp);

    } else if (strcasecmp(psFilterNode->pszValue, "NOT") == 0) {
      pszExpression =
        FLTGetLogicalComparisonSQLExpresssion(psFilterNode, lp);

    }
  }

  else if (psFilterNode->eType == FILTER_NODE_TYPE_SPATIAL) {
     
  } else if (psFilterNode->eType == FILTER_NODE_TYPE_FEATUREID) {
#if defined(USE_WMS_SVR) || defined (USE_WFS_SVR) || defined (USE_WCS_SVR) || defined(USE_SOS_SVR)
    if (psFilterNode->pszValue) {
      pszAttribute = msOWSLookupMetadata(&(lp->metadata), "OFG", "featureid");
      if (pszAttribute) {
        tokens = msStringSplit(psFilterNode->pszValue,',', &nTokens);
        bString = 0;
        if (tokens && nTokens > 0) {
          for (i=0; i<nTokens; i++) {
            char *pszEscapedStr = NULL;
            const char* pszId = tokens[i];
            const char* pszDot = strchr(pszId, '.');
            if( pszDot )
                pszId = pszDot + 1;

            if (strlen(pszId) <= 0)
              continue;

            if (FLTIsNumeric(pszId) == MS_FALSE)
              bString = 1;

            pszEscapedStr = msLayerEscapeSQLParam(lp, pszId);
            if (bString)
            {
              if( lp->connectiontype == MS_OGR || lp->connectiontype == MS_POSTGIS )
                snprintf(szTmp, sizeof(szTmp), "(CAST(%s AS CHARACTER(255)) = '%s')" , pszAttribute, pszEscapedStr);
              else
                snprintf(szTmp, sizeof(szTmp), "(%s = '%s')" , pszAttribute, pszEscapedStr);
            }
            else
              snprintf(szTmp, sizeof(szTmp), "(%s = %s)" , pszAttribute, pszEscapedStr);

            msFree(pszEscapedStr);
            pszEscapedStr=NULL;

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
               "FLTGetSQLExpression()");
    return(MS_FAILURE);
#endif

  }
  else if ( lp->connectiontype != MS_OGR &&
            psFilterNode->eType == FILTER_NODE_TYPE_TEMPORAL )
    pszExpression = FLTGetTimeExpression(psFilterNode, lp);

  return pszExpression;
}