int msPostGISLayerInitializeVirtualTable(layerObj *layer)
{
  assert(layer != NULL);
  assert(layer->vtable != NULL);

  layer->vtable->LayerInitItemInfo = msPostGISLayerInitItemInfo;
  layer->vtable->LayerFreeItemInfo = msPostGISLayerFreeItemInfo;
  layer->vtable->LayerOpen = msPostGISLayerOpen;
  layer->vtable->LayerIsOpen = msPostGISLayerIsOpen;
  layer->vtable->LayerWhichShapes = msPostGISLayerWhichShapes;
  layer->vtable->LayerNextShape = msPostGISLayerNextShape;
  layer->vtable->LayerGetShape = msPostGISLayerGetShape;
  layer->vtable->LayerClose = msPostGISLayerClose;
  layer->vtable->LayerGetItems = msPostGISLayerGetItems;
   
  layer->vtable->LayerApplyFilterToLayer = msLayerApplyCondSQLFilterToLayer;
   
   
  layer->vtable->LayerSetTimeFilter = msPostGISLayerSetTimeFilter;
   
   

   

  layer->vtable->LayerEscapeSQLParam = msPostGISEscapeSQLParam;
  layer->vtable->LayerEnablePaging = msPostGISEnablePaging;
  layer->vtable->LayerGetPaging = msPostGISGetPaging;

  return MS_SUCCESS;
}
