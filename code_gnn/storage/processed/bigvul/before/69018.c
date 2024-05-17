int FLTParseGMLEnvelope(CPLXMLNode *psRoot, rectObj *psBbox, char **ppszSRS)
{
  CPLXMLNode *psUpperCorner=NULL, *psLowerCorner=NULL;
  const char *pszLowerCorner=NULL, *pszUpperCorner=NULL;
  int bValid = 0;
  char **tokens;
  int n;

  if (psRoot && psBbox && psRoot->eType == CXT_Element &&
      EQUAL(psRoot->pszValue,"Envelope")) {
     
    if (ppszSRS) {
      const char* pszSRS = CPLGetXMLValue(psRoot, "srsName", NULL);
      if( pszSRS != NULL )
          *ppszSRS = msStrdup(pszSRS);
    }
    psLowerCorner = CPLSearchXMLNode(psRoot, "lowerCorner");
    psUpperCorner = CPLSearchXMLNode(psRoot, "upperCorner");

    if (psLowerCorner && psUpperCorner) {
      pszLowerCorner = CPLGetXMLValue(psLowerCorner, NULL, NULL);
      pszUpperCorner = CPLGetXMLValue(psUpperCorner, NULL, NULL);

      if (pszLowerCorner && pszUpperCorner) {
        tokens = msStringSplit(pszLowerCorner, ' ', &n);
        if (tokens && n >= 2) {
          psBbox->minx = atof(tokens[0]);
          psBbox->miny = atof(tokens[1]);

          msFreeCharArray(tokens, n);

          tokens = msStringSplit(pszUpperCorner, ' ', &n);
          if (tokens && n >= 2) {
            psBbox->maxx = atof(tokens[0]);
            psBbox->maxy = atof(tokens[1]);
            bValid = 1;
          }
        }
        msFreeCharArray(tokens, n);
      }
    }
  }

  return bValid;
}