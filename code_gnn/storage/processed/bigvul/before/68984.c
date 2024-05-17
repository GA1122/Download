char *FLTGetLogicalComparisonExpresssion(FilterEncodingNode *psFilterNode, layerObj *lp)
{
  char *pszTmp = NULL;
  char *pszBuffer = NULL;
  int nTmp = 0;

  if (!psFilterNode || !FLTIsLogicalFilterType(psFilterNode->pszValue))
    return NULL;


   
   
   
  if (psFilterNode->psLeftNode && psFilterNode->psRightNode &&
      (strcasecmp(psFilterNode->psLeftNode->pszValue, "BBOX") == 0 ||
       strcasecmp(psFilterNode->psRightNode->pszValue, "BBOX") == 0 ||
       FLTIsGeosNode(psFilterNode->psLeftNode->pszValue) ||
       FLTIsGeosNode(psFilterNode->psRightNode->pszValue)))


  {

     
    if (strcasecmp(psFilterNode->psLeftNode->pszValue, "BBOX") != 0 &&
        strcasecmp(psFilterNode->psLeftNode->pszValue, "DWithin") != 0 &&
        FLTIsGeosNode(psFilterNode->psLeftNode->pszValue) == MS_FALSE)
      pszTmp = FLTGetNodeExpression(psFilterNode->psLeftNode, lp);
    else
      pszTmp = FLTGetNodeExpression(psFilterNode->psRightNode, lp);

    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) * (strlen(pszTmp) + 3));
    pszBuffer[0] = '\0';
     
    sprintf(pszBuffer, "(%s)", pszTmp);

    free(pszTmp);

    return pszBuffer;
  }


   
   
   
  if (psFilterNode->psLeftNode && psFilterNode->psRightNode) {
    pszTmp = FLTGetNodeExpression(psFilterNode->psLeftNode, lp);
    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) *
                               (strlen(pszTmp) + strlen(psFilterNode->pszValue) + 5));
    pszBuffer[0] = '\0';
    strcat(pszBuffer, " (");

    strcat(pszBuffer, pszTmp);
    strcat(pszBuffer, " ");
    strcat(pszBuffer, psFilterNode->pszValue);
    strcat(pszBuffer, " ");
    free(pszTmp);

    pszTmp = FLTGetNodeExpression(psFilterNode->psRightNode, lp);
    if (!pszTmp) {
      msFree(pszBuffer);
      return NULL;
    }

    nTmp = strlen(pszBuffer);
    pszBuffer = (char *)realloc(pszBuffer,
                                sizeof(char) * (strlen(pszTmp) + nTmp +3));

    strcat(pszBuffer, pszTmp);
    strcat(pszBuffer, ") ");
    free(pszTmp);
  }
   
   
   
  else if (psFilterNode->psLeftNode &&
           strcasecmp(psFilterNode->pszValue, "NOT") == 0) {
    pszTmp = FLTGetNodeExpression(psFilterNode->psLeftNode, lp);
    if (!pszTmp)
      return NULL;

    pszBuffer = (char *)malloc(sizeof(char) *
                               (strlen(pszTmp) +  9));
    pszBuffer[0] = '\0';
    strcat(pszBuffer, " (NOT ");
    strcat(pszBuffer, pszTmp);
    strcat(pszBuffer, ") ");

    free(pszTmp);
  } else
    return NULL;

  return pszBuffer;

}