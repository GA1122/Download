  void RenderWidgetHostViewAura::InsertSyncPointAndACK(
    int32 route_id, int gpu_host_id, bool presented,
    ui::Compositor* compositor) {
//     const BufferPresentedParams& params) {
    uint32 sync_point = 0;
  if (compositor) {
    ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
    sync_point = factory->InsertSyncPoint();
//    
//    
//   if (params.texture_to_produce) {
//     params.texture_to_produce->Produce();
//     sync_point = ImageTransportFactory::GetInstance()->InsertSyncPoint();
    }
  
    RenderWidgetHostImpl::AcknowledgeBufferPresent(
      route_id, gpu_host_id, presented, sync_point);
//       params.route_id, params.gpu_host_id, params.surface_handle, sync_point);
  }