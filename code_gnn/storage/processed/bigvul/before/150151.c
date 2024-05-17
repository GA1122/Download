std::unique_ptr<ImageProcessorClient> ImageProcessorClient::Create(
    const ImageProcessor::PortConfig& input_config,
    const ImageProcessor::PortConfig& output_config,
    size_t num_buffers,
    std::vector<std::unique_ptr<VideoFrameProcessor>> frame_processors) {
  auto ip_client =
      base::WrapUnique(new ImageProcessorClient(std::move(frame_processors)));
  if (!ip_client->CreateImageProcessor(input_config, output_config,
                                       num_buffers)) {
    LOG(ERROR) << "Failed to create ImageProcessor";
    return nullptr;
  }
  return ip_client;
}
