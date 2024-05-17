int msPostGISLayerWhichShapes(layerObj *layer, rectObj rect, int isQuery)
{
#ifdef USE_POSTGIS
  msPostGISLayerInfo *layerinfo = NULL;
  char *strSQL = NULL;
  PGresult *pgresult = NULL;
  char** layer_bind_values = (char**)msSmallMalloc(sizeof(char*) * 1000);
  char* bind_value;
  char* bind_key = (char*)msSmallMalloc(3);

  int num_bind_values = 0;

   
  bind_value = msLookupHashTable(&layer->bindvals, "1");
  while(bind_value != NULL) {
     
    layer_bind_values[num_bind_values] = bind_value;
     
    num_bind_values++;
     
    sprintf(bind_key, "%d", num_bind_values+1);
     
    bind_value = msLookupHashTable(&layer->bindvals, bind_key);
  }

  assert(layer != NULL);
  assert(layer->layerinfo != NULL);

  if (layer->debug) {
    msDebug("msPostGISLayerWhichShapes called.\n");
  }

   
  if ( msPostGISParseData(layer) != MS_SUCCESS) {
    return MS_FAILURE;
  }

   
  layerinfo = (msPostGISLayerInfo*) layer->layerinfo;

   
  strSQL = msPostGISBuildSQL(layer, &rect, NULL);
  if ( ! strSQL ) {
    msSetError(MS_QUERYERR, "Failed to build query SQL.", "msPostGISLayerWhichShapes()");
    return MS_FAILURE;
  }

  if (layer->debug) {
    msDebug("msPostGISLayerWhichShapes query: %s\n", strSQL);
  }

  if(num_bind_values > 0) {
    pgresult = PQexecParams(layerinfo->pgconn, strSQL, num_bind_values, NULL, (const char**)layer_bind_values, NULL, NULL, 1);
  } else {
    pgresult = PQexecParams(layerinfo->pgconn, strSQL,0, NULL, NULL, NULL, NULL, 0);
  }

   
  free(bind_key);
  free(layer_bind_values);

  if ( layer->debug > 1 ) {
    msDebug("msPostGISLayerWhichShapes query status: %s (%d)\n", PQresStatus(PQresultStatus(pgresult)), PQresultStatus(pgresult));
  }

   
  if (!pgresult || PQresultStatus(pgresult) != PGRES_TUPLES_OK) {
    if ( layer->debug ) {
      msDebug("Error (%s) executing query: %s", "msPostGISLayerWhichShapes()\n", PQerrorMessage(layerinfo->pgconn), strSQL);
    }
    msSetError(MS_QUERYERR, "Error executing query: %s ", "msPostGISLayerWhichShapes()", PQerrorMessage(layerinfo->pgconn));
    free(strSQL);
    if (pgresult) {
      PQclear(pgresult);
    }
    return MS_FAILURE;
  }

  if ( layer->debug ) {
    msDebug("msPostGISLayerWhichShapes got %d records in result.\n", PQntuples(pgresult));
  }

   
  if(layerinfo->pgresult) PQclear(layerinfo->pgresult);
  layerinfo->pgresult = pgresult;

   
  if(layerinfo->sql) free(layerinfo->sql);
  layerinfo->sql = strSQL;

  layerinfo->rownum = 0;

  return MS_SUCCESS;
#else
  msSetError( MS_MISCERR,
              "PostGIS support is not available.",
              "msPostGISLayerWhichShapes()");
  return MS_FAILURE;
#endif
}
