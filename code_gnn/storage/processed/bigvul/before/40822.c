int msPostGISLayerGetItems(layerObj *layer)
{
#ifdef USE_POSTGIS
  msPostGISLayerInfo *layerinfo = NULL;
  static char *strSQLTemplate = "select * from %s where false limit 0";
  PGresult *pgresult = NULL;
  char *col = NULL;
  char *sql = NULL;
  char *strFrom = NULL;
  char found_geom = 0;
  const char *value;
  int t, item_num;
  rectObj rect;

   
  rect.minx = rect.miny = rect.maxx = rect.maxy = 0.0;

  assert(layer != NULL);
  assert(layer->layerinfo != NULL);

  layerinfo = (msPostGISLayerInfo*) layer->layerinfo;

  assert(layerinfo->pgconn);

  if (layer->debug) {
    msDebug("msPostGISLayerGetItems called.\n");
  }

   
  if ( msPostGISParseData(layer) != MS_SUCCESS) {
    return MS_FAILURE;
  }

  layerinfo = (msPostGISLayerInfo*) layer->layerinfo;

   
  strFrom = msPostGISReplaceBoxToken(layer, &rect, layerinfo->fromsource);

   
  sql = (char*) msSmallMalloc(strlen(strSQLTemplate) + strlen(strFrom));
  sprintf(sql, strSQLTemplate, strFrom);
  free(strFrom);

  if (layer->debug) {
    msDebug("msPostGISLayerGetItems executing SQL: %s\n", sql);
  }

  pgresult = PQexecParams(layerinfo->pgconn, sql,0, NULL, NULL, NULL, NULL, 0);

  if ( (!pgresult) || (PQresultStatus(pgresult) != PGRES_TUPLES_OK) ) {
    if ( layer->debug ) {
      msDebug("Error (%s) executing SQL: %s", "msPostGISLayerGetItems()\n", PQerrorMessage(layerinfo->pgconn), sql);
    }
    msSetError(MS_QUERYERR, "Error executing SQL: %s", "msPostGISLayerGetItems()", PQerrorMessage(layerinfo->pgconn));
    if (pgresult) {
      PQclear(pgresult);
    }
    free(sql);
    return MS_FAILURE;
  }

  free(sql);

  layer->numitems = PQnfields(pgresult) - 1;  
  layer->items = msSmallMalloc(sizeof(char*) * (layer->numitems + 1));  

  found_geom = 0;  
  item_num = 0;

  for (t = 0; t < PQnfields(pgresult); t++) {
    col = PQfname(pgresult, t);
    if ( strcmp(col, layerinfo->geomcolumn) != 0 ) {
       
      layer->items[item_num] = msStrdup(col);
      item_num++;
    } else {
      found_geom = 1;
    }
  }

   
  if((value = msOWSLookupMetadata(&(layer->metadata), "G", "types")) != NULL
      && strcasecmp(value,"auto") == 0 )
    msPostGISPassThroughFieldDefinitions( layer, pgresult );

   
  PQclear(pgresult);

  if (!found_geom) {
    msSetError(MS_QUERYERR, "Tried to find the geometry column in the database, but couldn't find it.  Is it mis-capitalized? '%s'", "msPostGISLayerGetItems()", layerinfo->geomcolumn);
    return MS_FAILURE;
  }

  return msPostGISLayerInitItemInfo(layer);
#else
  msSetError( MS_MISCERR,
              "PostGIS support is not available.",
              "msPostGISLayerGetItems()");
  return MS_FAILURE;
#endif
}
