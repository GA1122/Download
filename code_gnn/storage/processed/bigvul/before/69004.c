int FLTIsNumeric(const char *pszValue)
{
  if (pszValue != NULL && *pszValue != '\0' && !isspace(*pszValue)) {
     
#if defined(_WIN32) && !defined(__CYGWIN__)
    int i = 0, nLength=0, bString=0;

    nLength = strlen(pszValue);
    for (i=0; i<nLength; i++) {
      if (i == 0) {
        if (!isdigit(pszValue[i]) &&  pszValue[i] != '-') {
          bString = 1;
          break;
        }
      } else if (!isdigit(pszValue[i]) &&  pszValue[i] != '.') {
        bString = 1;
        break;
      }
    }
    if (!bString)
      return MS_TRUE;
#else
    char * p;
    strtod(pszValue, &p);
    if ( p != pszValue && *p == '\0') return MS_TRUE;
#endif
  }

  return MS_FALSE;
}
