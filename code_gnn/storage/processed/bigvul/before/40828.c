int msPostGISLayerOpen(layerObj *layer)
{
#ifdef USE_POSTGIS
  msPostGISLayerInfo  *layerinfo;
  int order_test = 1;

  assert(layer != NULL);

  if (layer->debug) {
    msDebug("msPostGISLayerOpen called: %s\n", layer->data);
  }

  if (layer->layerinfo) {
    if (layer->debug) {
      msDebug("msPostGISLayerOpen: Layer is already open!\n");
    }
    return MS_SUCCESS;   
  }

  if (!layer->data) {
    msSetError(MS_QUERYERR, "Nothing specified in DATA statement.", "msPostGISLayerOpen()");
    return MS_FAILURE;
  }

   
  layerinfo = msPostGISCreateLayerInfo();

  if (((char*) &order_test)[0] == 1) {
    layerinfo->endian = LITTLE_ENDIAN;
  } else {
    layerinfo->endian = BIG_ENDIAN;
  }

   
  layerinfo->pgconn = (PGconn *) msConnPoolRequest(layer);

   
  if (!layerinfo->pgconn) {
    char *conn_decrypted;
    if (layer->debug) {
      msDebug("msPostGISLayerOpen: No connection in pool, creating a fresh one.\n");
    }

    if (!layer->connection) {
      msSetError(MS_MISCERR, "Missing CONNECTION keyword.", "msPostGISLayerOpen()");
      return MS_FAILURE;
    }

     
    conn_decrypted = msDecryptStringTokens(layer->map, layer->connection);
    if (conn_decrypted == NULL) {
      return MS_FAILURE;   
    }
    layerinfo->pgconn = PQconnectdb(conn_decrypted);
    msFree(conn_decrypted);
    conn_decrypted = NULL;

     
    if (!layerinfo->pgconn || PQstatus(layerinfo->pgconn) == CONNECTION_BAD) {
      char *index, *maskeddata;
      if (layer->debug)
        msDebug("msPostGISLayerOpen: Connection failure.\n");

      maskeddata = msStrdup(layer->connection);
      index = strstr(maskeddata, "password=");
      if (index != NULL) {
        index = (char*)(index + 9);
        while (*index != '\0' && *index != ' ') {
          *index = '*';
          index++;
        }
      }

      msSetError(MS_QUERYERR, "Database connection failed (%s) with connect string '%s'\nIs the database running? Is it allowing connections? Does the specified user exist? Is the password valid? Is the database on the standard port?", "msPostGISLayerOpen()", PQerrorMessage(layerinfo->pgconn), maskeddata);

      free(maskeddata);
      free(layerinfo);
      return MS_FAILURE;
    }

     
    PQsetNoticeProcessor(layerinfo->pgconn, postresqlNoticeHandler, (void *) layer);

     
    msConnPoolRegister(layer, layerinfo->pgconn, msPostGISCloseConnection);
  } else {
     
    if( PQstatus(layerinfo->pgconn) != CONNECTION_OK ) {
       
      PQreset(layerinfo->pgconn);
      if( PQstatus(layerinfo->pgconn) != CONNECTION_OK ) {
         
        msSetError(MS_QUERYERR, "PostgreSQL database connection gone bad (%s)", "msPostGISLayerOpen()", PQerrorMessage(layerinfo->pgconn));
        return MS_FAILURE;
      }

    }
  }

   
  layerinfo->version = msPostGISRetrieveVersion(layerinfo->pgconn);
  if( layerinfo->version == MS_FAILURE ) return MS_FAILURE;
  if (layer->debug)
    msDebug("msPostGISLayerOpen: Got PostGIS version %d.\n", layerinfo->version);

   
  layer->layerinfo = (void*)layerinfo;

  return MS_SUCCESS;
#else
  msSetError( MS_MISCERR,
              "PostGIS support is not available.",
              "msPostGISLayerOpen()");
  return MS_FAILURE;
#endif
}
