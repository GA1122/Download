wkbConvGeometryToShape(wkbObj *w, shapeObj *shape)
{
  int wkbtype = wkbType(w);  

  switch(wkbtype) {
       
    case WKB_GEOMETRYCOLLECTION:
      return wkbConvCollectionToShape(w, shape);
       
    case WKB_POLYGON:
      return wkbConvPolygonToShape(w, shape);
    case WKB_MULTIPOLYGON:
      return wkbConvCollectionToShape(w, shape);
    case WKB_CURVEPOLYGON:
      return wkbConvCurvePolygonToShape(w, shape);
    case WKB_MULTISURFACE:
      return wkbConvCollectionToShape(w, shape);
  }

   
  if ( shape->type == MS_SHAPE_POLYGON ) return MS_FAILURE;

   
  switch(wkbtype) {
    case WKB_LINESTRING:
      return wkbConvLineStringToShape(w, shape);
    case WKB_CIRCULARSTRING:
      return wkbConvCircularStringToShape(w, shape);
    case WKB_COMPOUNDCURVE:
      return wkbConvCompoundCurveToShape(w, shape);
    case WKB_MULTILINESTRING:
      return wkbConvCollectionToShape(w, shape);
    case WKB_MULTICURVE:
      return wkbConvCollectionToShape(w, shape);
  }

   
  if ( shape->type == MS_SHAPE_LINE ) return MS_FAILURE;

   
  switch(wkbtype) {
    case WKB_POINT:
      return wkbConvPointToShape(w, shape);
    case WKB_MULTIPOINT:
      return wkbConvCollectionToShape(w, shape);
  }

   
  return MS_FAILURE;
}
