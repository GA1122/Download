char* FLTGetTimeExpression(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char* pszExpression = NULL;
  const char* pszTimeField;
  const char* pszTimeValue;

  if (psFilterNode == NULL || lp == NULL)
    return NULL;

  if (psFilterNode->eType != FILTER_NODE_TYPE_TEMPORAL)
    return NULL;

  pszTimeValue = FLTGetDuring(psFilterNode, &pszTimeField);
  if (pszTimeField && pszTimeValue) {
    expressionObj old_filter;
    msInitExpression(&old_filter);
    msCopyExpression(&old_filter, &lp->filter);  
    msFreeExpression(&lp->filter);
    if (msLayerSetTimeFilter(lp, pszTimeValue, pszTimeField) == MS_TRUE) {
      pszExpression = msStrdup(lp->filter.string);
    }
    msCopyExpression(&lp->filter, &old_filter);  
    msFreeExpression(&old_filter);
  }
  return pszExpression;
}