void PwnMessageHelper::FileSystemWrite(RenderProcessHost* process,
                                       int request_id,
                                       GURL file_path,
                                       std::string blob_uuid,
                                       int64_t position) {
  TestFileapiOperationWaiter waiter(
      process->GetStoragePartition()->GetFileSystemContext());

  IPC::IpcSecurityTestUtil::PwnMessageReceived(
      process->GetChannel(),
      FileSystemHostMsg_Write(request_id, file_path, blob_uuid, position));

  if (waiter.did_start_update())
    waiter.WaitForEndUpdate();
}
