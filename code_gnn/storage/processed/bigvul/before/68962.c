int FLTApplyExpressionToLayer(layerObj *lp, const char *pszExpression)
{
  char *pszFinalExpression=NULL, *pszBuffer = NULL;
   
  int bConcatWhere=0, bHasAWhere=0;

  if (lp && pszExpression) {
    if (lp->connectiontype == MS_POSTGIS || lp->connectiontype ==  MS_ORACLESPATIAL ||
        lp->connectiontype == MS_PLUGIN) {
      pszFinalExpression = msStrdup("(");
      pszFinalExpression = msStringConcatenate(pszFinalExpression, pszExpression);
      pszFinalExpression = msStringConcatenate(pszFinalExpression, ")");
    } else if (lp->connectiontype == MS_OGR) {
      pszFinalExpression = msStrdup(pszExpression);
      if (lp->filter.type != MS_EXPRESSION) {
        bConcatWhere = 1;
      } else {
        if (lp->filter.string && EQUALN(lp->filter.string,"WHERE ",6)) {
          bHasAWhere = 1;
          bConcatWhere =1;
        }
      }

    } else
      pszFinalExpression = msStrdup(pszExpression);

    if (bConcatWhere)
      pszBuffer = msStringConcatenate(pszBuffer, "WHERE ");
     
    if (lp->filter.string && lp->filter.type == MS_EXPRESSION) {
      pszBuffer = msStringConcatenate(pszBuffer, "((");
      if (bHasAWhere)
        pszBuffer = msStringConcatenate(pszBuffer, lp->filter.string+6);
      else
        pszBuffer = msStringConcatenate(pszBuffer, lp->filter.string);
      pszBuffer = msStringConcatenate(pszBuffer, ") and ");
    } else if (lp->filter.string)
      msFreeExpression(&lp->filter);

    pszBuffer = msStringConcatenate(pszBuffer, pszFinalExpression);

    if(lp->filter.string && lp->filter.type == MS_EXPRESSION)
      pszBuffer = msStringConcatenate(pszBuffer, ")");

     
    msLoadExpressionString(&lp->filter, pszBuffer);


    msFree(pszFinalExpression);

    if (pszBuffer)
      msFree(pszBuffer);

    return MS_TRUE;
  }

  return MS_FALSE;
}
