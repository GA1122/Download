bool BinaryUploadService::ShouldBlockFileSize(size_t file_size) {
  int block_large_file_transfer = g_browser_process->local_state()->GetInteger(
      prefs::kBlockLargeFileTransfer);
  if (block_large_file_transfer !=
          BlockLargeFileTransferValues::BLOCK_LARGE_DOWNLOADS &&
      block_large_file_transfer !=
          BlockLargeFileTransferValues::BLOCK_LARGE_UPLOADS_AND_DOWNLOADS)
    return false;

  return (file_size > kMaxUploadSizeBytes);
}
