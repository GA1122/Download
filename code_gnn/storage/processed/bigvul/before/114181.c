int32 BrowserGpuChannelHostFactory::CreateViewCommandBuffer(
      int32 surface_id,
      const GPUCreateCommandBufferConfig& init_params) {
  CreateRequest request;
  GetIOLoopProxy()->PostTask(FROM_HERE, base::Bind(
        &BrowserGpuChannelHostFactory::CreateViewCommandBufferOnIO,
        base::Unretained(this),
        &request,
        surface_id,
        init_params));
  request.event.Wait();
  return request.route_id;
}
