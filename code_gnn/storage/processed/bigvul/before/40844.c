wkbConvCollectionToShape(wkbObj *w, shapeObj *shape)
{
  int i, ncomponents;
  int failures = 0;

   wkbReadChar(w);
   wkbTypeMap(w,wkbReadInt(w));
  ncomponents = wkbReadInt(w);

   
  for ( i = 0; i < ncomponents; i++ ) {
    if ( wkbConvGeometryToShape(w, shape) == MS_FAILURE ) {
      wkbSkipGeometry(w);
      failures++;
    }
  }
  if ( failures == ncomponents )
    return MS_FAILURE;
  else
    return MS_SUCCESS;
}
