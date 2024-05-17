void PageCaptureSaveAsMHTMLFunction::CreateTemporaryFile() {
  bool success = base::CreateTemporaryFile(&mhtml_path_);
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::IO},
      base::BindOnce(&PageCaptureSaveAsMHTMLFunction::TemporaryFileCreatedOnIO,
                     this, success));
}
