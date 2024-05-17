char *msPostGISBuildSQLFrom(layerObj *layer, rectObj *rect)
{
  char *strFrom = 0;
  msPostGISLayerInfo *layerinfo;

  if (layer->debug) {
    msDebug("msPostGISBuildSQLFrom called.\n");
  }

  assert( layer->layerinfo != NULL);

  layerinfo = (msPostGISLayerInfo *)layer->layerinfo;

  if ( ! layerinfo->fromsource ) {
    msSetError(MS_MISCERR, "Layerinfo->fromsource is not initialized.", "msPostGISBuildSQLFrom()");
    return NULL;
  }

   
  strFrom = msPostGISReplaceBoxToken(layer, rect, layerinfo->fromsource);

  return strFrom;
}
