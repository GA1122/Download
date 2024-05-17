int FLTApplyFilterToLayerCommonExpressionWithRect(mapObj *map, int iLayerIndex, const char *pszExpression, rectObj rect)
{
  int retval;
  int save_startindex;
  int save_maxfeatures;
  int save_only_cache_result_count;

  save_startindex = map->query.startindex;
  save_maxfeatures = map->query.maxfeatures;
  save_only_cache_result_count = map->query.only_cache_result_count;
  msInitQuery(&(map->query));
  map->query.startindex = save_startindex;
  map->query.maxfeatures = save_maxfeatures;
  map->query.only_cache_result_count = save_only_cache_result_count;

  map->query.mode = MS_QUERY_MULTIPLE;
  map->query.layer = iLayerIndex;

  map->query.rect = rect;

  if( pszExpression )
  {
    map->query.type = MS_QUERY_BY_FILTER;
    msInitExpression(&map->query.filter);
    map->query.filter.string = msStrdup(pszExpression);
    map->query.filter.type = MS_EXPRESSION;  

    retval = msQueryByFilter(map);
  }
  else
  {
    map->query.type = MS_QUERY_BY_RECT;
    retval = msQueryByRect(map);
  }

  return retval;
}