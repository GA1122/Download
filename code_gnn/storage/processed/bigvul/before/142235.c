void UnblockFileTaskRunner() {
  GetLockForBlockingDefaultFileTaskRunner().Release();
}