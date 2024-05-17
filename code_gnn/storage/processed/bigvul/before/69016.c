FilterEncodingNode *FLTParseFilterEncoding(const char *szXMLString)
{
  CPLXMLNode *psRoot = NULL, *psChild=NULL, *psFilter=NULL;
  FilterEncodingNode *psFilterNode = NULL;

  if (szXMLString == NULL || strlen(szXMLString) <= 0 ||
      (strstr(szXMLString, "Filter") == NULL))
    return NULL;

  psRoot = CPLParseXMLString(szXMLString);

  if( psRoot == NULL)
    return NULL;

   
  CPLStripXMLNamespace(psRoot, NULL, 1);

   
   
   
  psFilter = CPLGetXMLNode(psRoot, "=Filter");
  if (!psFilter)
  {
    CPLDestroyXMLNode( psRoot );
    return NULL;
  }

  psChild = psFilter->psChild;
  while (psChild) {
    if (FLTIsSupportedFilterType(psChild)) {
      psFilterNode = FLTCreateFilterEncodingNode();
      FLTInsertElementInNode(psFilterNode, psChild);
      break;
    } else
      psChild = psChild->psNext;
  }

  CPLDestroyXMLNode( psRoot );

   
   
   
  if (!FLTValidFilterNode(psFilterNode)) {
    FLTFreeFilterEncodingNode(psFilterNode);
    return NULL;
  }


  return psFilterNode;
}
