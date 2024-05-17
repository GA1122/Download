bool TestURLLoader::Init() {
  if (!CheckTestingInterface()) {
    instance_->AppendError("Testing interface not available");
    return false;
  }

  const PPB_FileIO* file_io_interface = static_cast<const PPB_FileIO*>(
      pp::Module::Get()->GetBrowserInterface(PPB_FILEIO_INTERFACE));
  if (!file_io_interface)
    instance_->AppendError("FileIO interface not available");

  file_io_private_interface_ = static_cast<const PPB_FileIO_Private*>(
      pp::Module::Get()->GetBrowserInterface(PPB_FILEIO_PRIVATE_INTERFACE));
  if (!file_io_private_interface_)
    instance_->AppendError("FileIO_Private interface not available");
  url_loader_trusted_interface_ = static_cast<const PPB_URLLoaderTrusted*>(
      pp::Module::Get()->GetBrowserInterface(PPB_URLLOADERTRUSTED_INTERFACE));
  if (!testing_interface_->IsOutOfProcess()) {
#if !(defined __native_client__)
    if (!url_loader_trusted_interface_)
      instance_->AppendError("URLLoaderTrusted interface not available");
#else
    if (url_loader_trusted_interface_)
      instance_->AppendError("URLLoaderTrusted interface is supported by NaCl");
#endif
  }
  return EnsureRunningOverHTTP();
}
