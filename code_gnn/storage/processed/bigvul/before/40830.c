int msPostGISParseData(layerObj *layer)
{
  char *pos_opt, *pos_scn, *tmp, *pos_srid, *pos_uid, *pos_geom, *data;
  int slength;
  msPostGISLayerInfo *layerinfo;

  assert(layer != NULL);
  assert(layer->layerinfo != NULL);

  layerinfo = (msPostGISLayerInfo*)(layer->layerinfo);

  if (layer->debug) {
    msDebug("msPostGISParseData called.\n");
  }

  if (!layer->data) {
    msSetError(MS_QUERYERR, "Missing DATA clause. DATA statement must contain 'geometry_column from table_name' or 'geometry_column from (sub-query) as sub'.", "msPostGISParseData()");
    return MS_FAILURE;
  }
  data = layer->data;

   
  if( layerinfo->srid ) {
    free(layerinfo->srid);
    layerinfo->srid = NULL;
  }
  if( layerinfo->uid ) {
    free(layerinfo->uid);
    layerinfo->uid = NULL;
  }
  if( layerinfo->geomcolumn ) {
    free(layerinfo->geomcolumn);
    layerinfo->geomcolumn = NULL;
  }
  if( layerinfo->fromsource ) {
    free(layerinfo->fromsource);
    layerinfo->fromsource = NULL;
  }

   
  pos_uid = strcasestr(data, " using unique ");
  if (pos_uid) {
     
    tmp = strstr(pos_uid + 14, " ");
     
    if (!tmp) {
      tmp = pos_uid + strlen(pos_uid);
    }
    layerinfo->uid = (char*) msSmallMalloc((tmp - (pos_uid + 14)) + 1);
    strlcpy(layerinfo->uid, pos_uid + 14, tmp - (pos_uid + 14)+1);
    msStringTrim(layerinfo->uid);
  }

   
  pos_srid = strcasestr(data, " using srid=");
  if (!pos_srid) {
    layerinfo->srid = (char*) msSmallMalloc(1);
    (layerinfo->srid)[0] = '\0';  
  } else {
    slength = strspn(pos_srid + 12, "-0123456789");
    if (!slength) {
      msSetError(MS_QUERYERR, "Error parsing PostGIS DATA variable. You specified 'USING SRID' but didnt have any numbers! %s", "msPostGISParseData()", data);
      return MS_FAILURE;
    } else {
      layerinfo->srid = (char*) msSmallMalloc(slength + 1);
      strlcpy(layerinfo->srid, pos_srid + 12, slength+1);
      msStringTrim(layerinfo->srid);
    }
  }

   
  if (pos_srid && pos_uid) {
    pos_opt = (pos_srid > pos_uid) ? pos_uid : pos_srid;
  }
   
  else {
    pos_opt = (pos_srid > pos_uid) ? pos_srid : pos_uid;
  }
   
  if (!pos_opt) {
    pos_opt = data + strlen(data);
  }

   

   
  pos_geom = data;
  while( *pos_geom == ' ' || *pos_geom == '\t' || *pos_geom == '\n' || *pos_geom == '\r' )
    pos_geom++;

   
  pos_scn = strcasestr(data, " from ");
  if (!pos_scn) {
    msSetError(MS_QUERYERR, "Error parsing PostGIS DATA variable. Must contain 'geometry from table' or 'geometry from (subselect) as foo'. %s", "msPostGISParseData()", data);
    return MS_FAILURE;
  }

   
  layerinfo->geomcolumn = (char*) msSmallMalloc((pos_scn - pos_geom) + 1);
  strlcpy(layerinfo->geomcolumn, pos_geom, pos_scn - pos_geom+1);
  msStringTrim(layerinfo->geomcolumn);

   
  layerinfo->fromsource = (char*) msSmallMalloc((pos_opt - (pos_scn + 6)) + 1);
  strlcpy(layerinfo->fromsource, pos_scn + 6, pos_opt - (pos_scn + 6)+1);
  msStringTrim(layerinfo->fromsource);

   
  if (strlen(layerinfo->fromsource) < 1 || strlen(layerinfo->geomcolumn) < 1) {
    msSetError(MS_QUERYERR, "Error parsing PostGIS DATA variable.  Must contain 'geometry from table' or 'geometry from (subselect) as foo'. %s", "msPostGISParseData()", data);
    return MS_FAILURE;
  }

   
  if ( ! (layerinfo->uid) ) {
    if ( strstr(layerinfo->fromsource, " ") ) {
      msSetError(MS_QUERYERR, "Error parsing PostGIS DATA variable.  You must specify 'using unique' when supplying a subselect in the data definition.", "msPostGISParseData()");
      return MS_FAILURE;
    }
    if ( msPostGISRetrievePK(layer) != MS_SUCCESS ) {
       
       
      layerinfo->uid = msStrdup("oid");
    }
  }

  if (layer->debug) {
    msDebug("msPostGISParseData: unique_column=%s, srid=%s, geom_column_name=%s, table_name=%s\n", layerinfo->uid, layerinfo->srid, layerinfo->geomcolumn, layerinfo->fromsource);
  }
  return MS_SUCCESS;
}
