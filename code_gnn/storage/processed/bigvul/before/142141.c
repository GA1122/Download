void BlockFileTaskRunner(Profile* profile) {
  GetLockForBlockingDefaultFileTaskRunner().Acquire();

  content::BrowserContext::GetDefaultStoragePartition(profile)
      ->GetFileSystemContext()
      ->default_file_task_runner()
      ->PostTask(FROM_HERE, base::BindOnce([] {
                   base::AutoLock l(GetLockForBlockingDefaultFileTaskRunner());
                 }));
}
