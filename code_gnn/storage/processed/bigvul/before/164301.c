void PageCaptureSaveAsMHTMLFunction::TemporaryFileCreatedOnIO(bool success) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (success) {
    mhtml_file_ = ShareableFileReference::GetOrCreate(
        mhtml_path_, ShareableFileReference::DELETE_ON_FINAL_RELEASE,
        base::CreateSequencedTaskRunnerWithTraits(
            { 
             base::MayBlock(),


             base::TaskShutdownBehavior::BLOCK_SHUTDOWN})
            .get());
  }
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&PageCaptureSaveAsMHTMLFunction::TemporaryFileCreatedOnUI,
                     this, success));
}
