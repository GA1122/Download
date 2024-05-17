void FLTPreParseFilterForAliasAndGroup(FilterEncodingNode *psFilterNode,
                               mapObj *map, int i, const char *namespaces)
{
  layerObj *lp=NULL;
  char szTmp[256];
  const char *pszFullName = NULL;
  int layerWasOpened =  MS_FALSE;

#if defined(USE_WMS_SVR) || defined (USE_WFS_SVR) || defined (USE_WCS_SVR) || defined(USE_SOS_SVR)

  if (psFilterNode && map && i>=0 && i<map->numlayers) {
     
    FLTStripNameSpacesFromPropertyName(psFilterNode);

    lp = GET_LAYER(map, i);
    layerWasOpened = msLayerIsOpen(lp);
    if (msLayerOpen(lp) == MS_SUCCESS && msLayerGetItems(lp) == MS_SUCCESS) {

       
      gmlGroupListObj* groupList = msGMLGetGroups(lp, namespaces);
      if( groupList && groupList->numgroups > 0 )
        FLTRemoveGroupName(psFilterNode, groupList);
      msGMLFreeGroups(groupList);

      for(i=0; i<lp->numitems; i++) {
        if (!lp->items[i] || strlen(lp->items[i]) <= 0)
          continue;
        snprintf(szTmp, sizeof(szTmp), "%s_alias", lp->items[i]);
        pszFullName = msOWSLookupMetadata(&(lp->metadata), namespaces, szTmp);
        if (pszFullName) {
          FLTReplacePropertyName(psFilterNode, pszFullName,
                                 lp->items[i]);
        }
      }
      if (!layerWasOpened)  
        msLayerClose(lp);
    }
  }
#else
  msSetError(MS_MISCERR, "OWS support is not available.",
             "FLTPreParseFilterForAlias()");

#endif
}