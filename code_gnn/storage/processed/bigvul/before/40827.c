int msPostGISLayerNextShape(layerObj *layer, shapeObj *shape)
{
#ifdef USE_POSTGIS
  msPostGISLayerInfo  *layerinfo;

  if (layer->debug) {
    msDebug("msPostGISLayerNextShape called.\n");
  }

  assert(layer != NULL);
  assert(layer->layerinfo != NULL);

  layerinfo = (msPostGISLayerInfo*) layer->layerinfo;

  shape->type = MS_SHAPE_NULL;

   
  while (shape->type == MS_SHAPE_NULL) {
    if (layerinfo->rownum < PQntuples(layerinfo->pgresult)) {
       
      msPostGISReadShape(layer, shape);
      if( shape->type != MS_SHAPE_NULL ) {
        (layerinfo->rownum)++;  
        return MS_SUCCESS;
      } else {
        (layerinfo->rownum)++;  
      }
    } else {
      return MS_DONE;
    }
  }

   
  msFreeShape(shape);

  return MS_FAILURE;
#else
  msSetError( MS_MISCERR,
              "PostGIS support is not available.",
              "msPostGISLayerNextShape()");
  return MS_FAILURE;
#endif
}
