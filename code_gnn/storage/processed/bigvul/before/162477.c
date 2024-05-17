void ImageResource::OnMemoryDump(WebMemoryDumpLevelOfDetail level_of_detail,
                                 WebProcessMemoryDump* memory_dump) const {
  Resource::OnMemoryDump(level_of_detail, memory_dump);
  const String name = GetMemoryDumpName() + "/image_content";
  auto* dump = memory_dump->CreateMemoryAllocatorDump(name);
  size_t encoded_size =
      content_->HasImage() ? content_->GetImage()->Data()->size() : 0;
  dump->AddScalar("size", "bytes", encoded_size);
}
