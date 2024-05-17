wkbConvCompoundCurveToShape(wkbObj *w, shapeObj *shape)
{
  int npoints = 0;
  int type, ncomponents, i, j;
  lineObj *line;
  shapeObj shapebuf;

   wkbReadChar(w);
  type = wkbTypeMap(w,wkbReadInt(w));

   
  msInitShape(&shapebuf);

  if( type != WKB_COMPOUNDCURVE ) return MS_FAILURE;

   
  ncomponents = wkbReadInt(w);

   
  for( i = 0; i < ncomponents; i++ )
    wkbConvGeometryToShape(w, &shapebuf);

   
  if ( shapebuf.numlines == 0 )
    return MS_FAILURE;

   
  for( i = 0; i < shapebuf.numlines; i++ )
    npoints += shapebuf.line[i].numpoints;

   
  if ( npoints == 0 )
    return MS_FAILURE;

   
  line = msSmallMalloc(sizeof(lineObj));
  line->numpoints = npoints;
  line->point = msSmallMalloc(sizeof(pointObj) * npoints);

   
  npoints = 0;
  for ( i = 0; i < shapebuf.numlines; i++ ) {
    for ( j = 0; j < shapebuf.line[i].numpoints; j++ ) {
       
      if( j == 0 && i > 0 &&
          memcmp(&(line->point[npoints - 1]),&(shapebuf.line[i].point[j]),sizeof(pointObj)) == 0 ) {
        continue;
      }
      line->point[npoints++] = shapebuf.line[i].point[j];
    }
  }
  line->numpoints = npoints;

   
  msFreeShape(&shapebuf);

   
  msAddLineDirectly(shape, line);

  return MS_SUCCESS;
}
