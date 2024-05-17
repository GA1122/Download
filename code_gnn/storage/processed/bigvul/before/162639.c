void HeadlessPrintManager::OnDidPrintDocument(
    const PrintHostMsg_DidPrintDocument_Params& params) {
  if (!base::SharedMemory::IsHandleValid(params.metafile_data_handle)) {
    ReleaseJob(INVALID_MEMORY_HANDLE);
    return;
  }
  auto shared_buf =
      std::make_unique<base::SharedMemory>(params.metafile_data_handle, true);
  if (!shared_buf->Map(params.data_size)) {
    ReleaseJob(METAFILE_MAP_ERROR);
    return;
  }
  data_ = std::string(static_cast<const char*>(shared_buf->memory()),
                      params.data_size);
  ReleaseJob(PRINT_SUCCESS);
}
