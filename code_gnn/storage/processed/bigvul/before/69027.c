int FLTValidForBBoxFilter(FilterEncodingNode *psFilterNode)
{
  int nCount = 0;

  if (!psFilterNode || !psFilterNode->pszValue)
    return 1;

  nCount = FLTNumberOfFilterType(psFilterNode, "BBOX");

  if (nCount > 1)
    return 0;
  else if (nCount == 0)
    return 1;

   
  if (strcasecmp(psFilterNode->pszValue, "BBOX") == 0)
    return 1;

  if (strcasecmp(psFilterNode->pszValue, "AND") == 0) {
    return FLTValidForBBoxFilter(psFilterNode->psLeftNode) &&
           FLTValidForBBoxFilter(psFilterNode->psRightNode);
  }

  return 0;
}