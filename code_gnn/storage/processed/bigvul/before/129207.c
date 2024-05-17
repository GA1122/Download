base::Closure GLES2DecoderImpl::AsyncUploadTokenCompletionClosure(
    uint32 async_upload_token,
    uint32 sync_data_shm_id,
    uint32 sync_data_shm_offset) {
  scoped_refptr<gpu::Buffer> buffer = GetSharedMemoryBuffer(sync_data_shm_id);
  if (!buffer || !buffer->GetDataAddress(sync_data_shm_offset,
                                         sizeof(AsyncUploadSync)))
    return base::Closure();

  AsyncMemoryParams mem_params(buffer,
                               sync_data_shm_offset,
                               sizeof(AsyncUploadSync));

  scoped_refptr<AsyncUploadTokenCompletionObserver> observer(
      new AsyncUploadTokenCompletionObserver(async_upload_token));

  return base::Bind(
      &AsyncPixelTransferManager::AsyncNotifyCompletion,
      base::Unretained(GetAsyncPixelTransferManager()),
      mem_params,
      observer);
}
