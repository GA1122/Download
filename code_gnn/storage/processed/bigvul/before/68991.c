shapeObj *FLTGetShape(FilterEncodingNode *psFilterNode, double *pdfDistance,
                      int *pnUnit)
{
  char **tokens = NULL;
  int nTokens = 0;
  FilterEncodingNode *psNode = psFilterNode;
  char *szUnitStr = NULL;
  char *szUnit = NULL;

  if (psNode) {
    if (psNode->eType == FILTER_NODE_TYPE_SPATIAL && psNode->psRightNode)
      psNode = psNode->psRightNode;

    if (FLTIsGeometryFilterNodeType(psNode->eType)) {

      if (psNode->pszValue && pdfDistance) {
         
        tokens = msStringSplit(psNode->pszValue,';', &nTokens);
        if (tokens && nTokens >= 1) {
          *pdfDistance = atof(tokens[0]);

          if (nTokens == 2 && pnUnit) {
            szUnitStr = msStrdup(tokens[1]);
            msFreeCharArray(tokens, nTokens);
            nTokens = 0;
            tokens = msStringSplit(szUnitStr,'#', &nTokens);
            msFree(szUnitStr);
            if (tokens && nTokens >= 1) {
              if (nTokens ==1)
                szUnit = tokens[0];
              else
                szUnit = tokens[1];

              if (strcasecmp(szUnit,"m") == 0 ||
                  strcasecmp(szUnit,"meters") == 0 )
                *pnUnit = MS_METERS;
              else if (strcasecmp(szUnit,"km") == 0 ||
                       strcasecmp(szUnit,"kilometers") == 0)
                *pnUnit = MS_KILOMETERS;
              else if (strcasecmp(szUnit,"NM") == 0 ||
                       strcasecmp(szUnit,"nauticalmiles") == 0)
                *pnUnit = MS_NAUTICALMILES;
              else if (strcasecmp(szUnit,"mi") == 0 ||
                       strcasecmp(szUnit,"miles") == 0)
                *pnUnit = MS_MILES;
              else if (strcasecmp(szUnit,"in") == 0 ||
                       strcasecmp(szUnit,"inches") == 0)
                *pnUnit = MS_INCHES;
              else if (strcasecmp(szUnit,"ft") == 0 ||
                       strcasecmp(szUnit,"feet") == 0)
                *pnUnit = MS_FEET;
              else if (strcasecmp(szUnit,"deg") == 0 ||
                       strcasecmp(szUnit,"dd") == 0)
                *pnUnit = MS_DD;
              else if (strcasecmp(szUnit,"px") == 0)
                *pnUnit = MS_PIXELS;

            }
          }
        } 
        msFreeCharArray(tokens, nTokens);
      }

      return (shapeObj *)psNode->pOther;
    }
  }
  return NULL;
}