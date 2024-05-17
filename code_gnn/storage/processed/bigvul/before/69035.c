char *FLTGetLogicalComparisonCommonExpression(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char *pszExpression = NULL;
  char *pszTmp = NULL;

  if (!psFilterNode || !FLTIsLogicalFilterType(psFilterNode->pszValue))
    return NULL;

   
   
   
  if (psFilterNode->psLeftNode && psFilterNode->psRightNode) {
    pszTmp = FLTGetCommonExpression(psFilterNode->psLeftNode, lp);
    if (!pszTmp)
      return NULL;

    pszExpression = msStringConcatenate(pszExpression, "(");

    pszExpression = msStringConcatenate(pszExpression, pszTmp);
    msFree(pszTmp);

    pszExpression = msStringConcatenate(pszExpression, " ");

    pszExpression = msStringConcatenate(pszExpression, psFilterNode->pszValue);

    pszExpression = msStringConcatenate(pszExpression, " ");

    pszTmp = FLTGetCommonExpression(psFilterNode->psRightNode, lp);
    if (!pszTmp) {
      msFree(pszExpression);
      return NULL;
    }

    pszExpression = msStringConcatenate(pszExpression, pszTmp);
    msFree(pszTmp);

    pszExpression = msStringConcatenate(pszExpression, ")");
  }
   
   
   
  else if (psFilterNode->psLeftNode && strcasecmp(psFilterNode->pszValue, "NOT") == 0) {
    pszTmp = FLTGetCommonExpression(psFilterNode->psLeftNode, lp);
    if (!pszTmp)
      return NULL;

    pszExpression = msStringConcatenate(pszExpression, "(NOT ");

    pszExpression = msStringConcatenate(pszExpression, pszTmp);
    msFree(pszTmp);

    pszExpression = msStringConcatenate(pszExpression, ")");
  }

  return pszExpression;
}
