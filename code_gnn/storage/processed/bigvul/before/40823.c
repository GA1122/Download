int msPostGISLayerGetShape(layerObj *layer, shapeObj *shape, resultObj *record)
{
#ifdef USE_POSTGIS

  PGresult *pgresult = NULL;
  msPostGISLayerInfo *layerinfo = NULL;

  long shapeindex = record->shapeindex;
  int resultindex = record->resultindex;

  assert(layer != NULL);
  assert(layer->layerinfo != NULL);

  if (layer->debug) {
    msDebug("msPostGISLayerGetShape called for record = %i\n", resultindex);
  }

   
  if (resultindex >= 0) {
    int status;

    layerinfo = (msPostGISLayerInfo*) layer->layerinfo;

     
    pgresult = layerinfo->pgresult;
    if ( ! pgresult ) {
      msSetError( MS_MISCERR,
                  "PostgreSQL result set is null.",
                  "msPostGISLayerGetShape()");
      return MS_FAILURE;
    }
    status = PQresultStatus(pgresult);
    if ( layer->debug > 1 ) {
      msDebug("msPostGISLayerGetShape query status: %s (%d)\n", PQresStatus(status), status);
    }
    if( ! ( status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK) ) {
      msSetError( MS_MISCERR,
                  "PostgreSQL result set is not ready.",
                  "msPostGISLayerGetShape()");
      return MS_FAILURE;
    }

     
    if( resultindex >= PQntuples(pgresult) ) {
      msDebug("msPostGISLayerGetShape got request for (%d) but only has %d tuples.\n", resultindex, PQntuples(pgresult));
      msSetError( MS_MISCERR,
                  "Got request larger than result set.",
                  "msPostGISLayerGetShape()");
      return MS_FAILURE;
    }

    layerinfo->rownum = resultindex;  

     
    shape->type = MS_SHAPE_NULL;

     
    msPostGISReadShape(layer, shape);

    return (shape->type == MS_SHAPE_NULL) ? MS_FAILURE : MS_SUCCESS;
  } else {  
    int num_tuples;
    char *strSQL = 0;

     
    if ( msPostGISParseData(layer) != MS_SUCCESS) {
      return MS_FAILURE;
    }

     
    layerinfo = (msPostGISLayerInfo*) layer->layerinfo;

     
    strSQL = msPostGISBuildSQL(layer, 0, &shapeindex);
    if ( ! strSQL ) {
      msSetError(MS_QUERYERR, "Failed to build query SQL.", "msPostGISLayerGetShape()");
      return MS_FAILURE;
    }

    if (layer->debug) {
      msDebug("msPostGISLayerGetShape query: %s\n", strSQL);
    }

    pgresult = PQexecParams(layerinfo->pgconn, strSQL,0, NULL, NULL, NULL, NULL, 0);

     
    if ( (!pgresult) || (PQresultStatus(pgresult) != PGRES_TUPLES_OK) ) {
      if ( layer->debug ) {
        msDebug("Error (%s) executing SQL: %s", "msPostGISLayerGetShape()\n", PQerrorMessage(layerinfo->pgconn), strSQL );
      }
      msSetError(MS_QUERYERR, "Error executing SQL: %s", "msPostGISLayerGetShape()", PQerrorMessage(layerinfo->pgconn));

      if (pgresult) {
        PQclear(pgresult);
      }
      free(strSQL);

      return MS_FAILURE;
    }

     
    if(layerinfo->pgresult) PQclear(layerinfo->pgresult);
    layerinfo->pgresult = pgresult;

     
    if(layerinfo->sql) free(layerinfo->sql);
    layerinfo->sql = strSQL;

    layerinfo->rownum = 0;  

     
    shape->type = MS_SHAPE_NULL;

    num_tuples = PQntuples(pgresult);
    if (layer->debug) {
      msDebug("msPostGISLayerGetShape number of records: %d\n", num_tuples);
    }

    if (num_tuples > 0) {
       
      msPostGISReadShape(layer, shape);
    }

    return (shape->type == MS_SHAPE_NULL) ? MS_FAILURE : ( (num_tuples > 0) ? MS_SUCCESS : MS_DONE );
  }
#else
  msSetError( MS_MISCERR,
              "PostGIS support is not available.",
              "msPostGISLayerGetShape()");
  return MS_FAILURE;
#endif
}
