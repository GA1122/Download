void BrowserGpuChannelHostFactory::CommandBufferCreatedOnIO(
    CreateRequest* request, int32 route_id) {
  request->route_id = route_id;
  request->event.Signal();
}
