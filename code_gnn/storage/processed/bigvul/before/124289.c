ExtensionInfoMap* ExtensionSystemImpl::Shared::info_map() {
  if (!extension_info_map_)
    extension_info_map_ = new ExtensionInfoMap();
  return extension_info_map_.get();
}
