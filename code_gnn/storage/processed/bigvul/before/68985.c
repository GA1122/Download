char *FLTGetLogicalComparisonSQLExpresssion(FilterEncodingNode *psFilterNode,
    layerObj *lp)
{
  char *pszBuffer = NULL;
  char *pszTmp = NULL;
  int nTmp = 0;

  if (lp == NULL)
    return NULL;

   
   
   
  if (psFilterNode->psLeftNode && psFilterNode->psRightNode &&
      ((strcasecmp(psFilterNode->psLeftNode->pszValue, "BBOX") == 0) ||
       (strcasecmp(psFilterNode->psRightNode->pszValue, "BBOX") == 0))) {
    if (strcasecmp(psFilterNode->psLeftNode->pszValue, "BBOX") != 0)
      pszTmp = FLTGetSQLExpression(psFilterNode->psLeftNode, lp);
    else
      pszTmp = FLTGetSQLExpression(psFilterNode->psRightNode, lp);

    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) * (strlen(pszTmp) + 1));
    sprintf(pszBuffer, "%s", pszTmp);
  }

   
   
   
  else if (lp->connectiontype == MS_OGR &&
      psFilterNode->psLeftNode && psFilterNode->psRightNode &&
      (psFilterNode->psLeftNode->eType == FILTER_NODE_TYPE_TEMPORAL ||
       psFilterNode->psRightNode->eType == FILTER_NODE_TYPE_TEMPORAL) ) {
    if (psFilterNode->psLeftNode->eType != FILTER_NODE_TYPE_TEMPORAL)
      pszTmp = FLTGetSQLExpression(psFilterNode->psLeftNode, lp);
    else
      pszTmp = FLTGetSQLExpression(psFilterNode->psRightNode, lp);

    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) * (strlen(pszTmp) + 1));
    sprintf(pszBuffer, "%s", pszTmp);
  }
  
   
   
   
  else if (psFilterNode->psLeftNode && psFilterNode->psRightNode) {
    pszTmp = FLTGetSQLExpression(psFilterNode->psLeftNode, lp);
    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) *
                               (strlen(pszTmp) +
                                strlen(psFilterNode->pszValue) + 5));
    pszBuffer[0] = '\0';
    strcat(pszBuffer, " (");
    strcat(pszBuffer, pszTmp);
    strcat(pszBuffer, " ");
    strcat(pszBuffer, psFilterNode->pszValue);
    strcat(pszBuffer, " ");

    free( pszTmp );

    nTmp = strlen(pszBuffer);
    pszTmp = FLTGetSQLExpression(psFilterNode->psRightNode, lp);
    if (!pszTmp) {
      free(pszBuffer);
      return NULL;
    }

    pszBuffer = (char *)realloc(pszBuffer,
                                sizeof(char) * (strlen(pszTmp) + nTmp +3));
    strcat(pszBuffer, pszTmp);
    strcat(pszBuffer, ") ");
  }
   
   
   
  else if (psFilterNode->psLeftNode &&
           strcasecmp(psFilterNode->pszValue, "NOT") == 0) {
    pszTmp = FLTGetSQLExpression(psFilterNode->psLeftNode, lp);
    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) * (strlen(pszTmp) +  9));
    pszBuffer[0] = '\0';

    strcat(pszBuffer, " (NOT ");
    strcat(pszBuffer, pszTmp);
    strcat(pszBuffer, ") ");
  } else
    return NULL;

   
   
   
  if( pszTmp != NULL )
    free( pszTmp );
  return pszBuffer;

}