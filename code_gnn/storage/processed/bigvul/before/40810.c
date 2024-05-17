char *msPostGISBuildSQLSRID(layerObj *layer)
{

  char *strSRID = NULL;
  msPostGISLayerInfo *layerinfo = NULL;

  if (layer->debug) {
    msDebug("msPostGISBuildSQLSRID called.\n");
  }

  assert( layer->layerinfo != NULL);

  layerinfo = (msPostGISLayerInfo *)layer->layerinfo;

   
  if ( layerinfo->srid && (strlen(layerinfo->srid) > 0) ) {
    strSRID = msStrdup(layerinfo->srid);
    if( layer->debug > 1 ) {
      msDebug("msPostGISBuildSQLSRID: SRID provided (%s)\n", strSRID);
    }
  }
   
  else {
    char *f_table_name;
    char *strSRIDTemplate = "find_srid('','%s','%s')";
    char *pos = strstr(layerinfo->fromsource, " ");
    if( layer->debug > 1 ) {
      msDebug("msPostGISBuildSQLSRID: Building find_srid line.\n", strSRID);
    }

    if ( ! pos ) {
       
      f_table_name = msStrdup(layerinfo->fromsource);
      if( layer->debug > 1 ) {
        msDebug("msPostGISBuildSQLSRID: Found table (%s)\n", f_table_name);
      }
    } else {
       
      pos = strcasestr(layerinfo->fromsource, " from ");
      if ( pos ) {
        char *pos_paren;
        char *pos_space;
        pos += 6;  
        pos_paren = strstr(pos, ")");  
        pos_space = strstr(pos, " ");  
        if ( pos_space < pos_paren ) {
           
          f_table_name = (char*)msSmallMalloc(pos_space - pos + 1);
          strlcpy(f_table_name, pos, pos_space - pos+1);
        } else {
           
          f_table_name = (char*)msSmallMalloc(pos_paren - pos + 1);
          strlcpy(f_table_name, pos, pos_paren - pos+1);
        }
      } else {
         
        return NULL;
      }
    }
    strSRID = msSmallMalloc(strlen(strSRIDTemplate) + strlen(f_table_name) + strlen(layerinfo->geomcolumn));
    sprintf(strSRID, strSRIDTemplate, f_table_name, layerinfo->geomcolumn);
    if ( f_table_name ) free(f_table_name);
  }
  return strSRID;
}
