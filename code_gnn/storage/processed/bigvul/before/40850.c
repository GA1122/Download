wkbConvPolygonToShape(wkbObj *w, shapeObj *shape)
{
  int type;
  int i, nrings;
  lineObj line;

   wkbReadChar(w);
  type = wkbTypeMap(w,wkbReadInt(w));

  if( type != WKB_POLYGON ) return MS_FAILURE;

   
  nrings = wkbReadInt(w);

   
  for( i = 0; i < nrings; i++ ) {
    wkbReadLine(w,&line);
    msAddLineDirectly(shape, &line);
  }

  return MS_SUCCESS;
}
