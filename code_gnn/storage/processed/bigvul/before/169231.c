void PwnMessageHelper::FileSystemCreate(RenderProcessHost* process,
                                        int request_id,
                                        GURL path,
                                        bool exclusive,
                                        bool is_directory,
                                        bool recursive) {
  TestFileapiOperationWaiter waiter(
      process->GetStoragePartition()->GetFileSystemContext());

  IPC::IpcSecurityTestUtil::PwnMessageReceived(
      process->GetChannel(),
      FileSystemHostMsg_Create(request_id, path, exclusive, is_directory,
                               recursive));

  if (waiter.did_start_update())
    waiter.WaitForEndUpdate();
}
