std::unique_ptr<MultiBuffer::DataProvider> ResourceMultiBuffer::CreateWriter(
    const MultiBufferBlockId& pos,
    bool is_client_audio_element) {
  auto writer = std::make_unique<ResourceMultiBufferDataProvider>(
      url_data_, pos, is_client_audio_element);
  writer->Start();
  return writer;
}
