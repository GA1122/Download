void ChromeDownloadManagerDelegate::GetFileMimeType(
    const base::FilePath& path,
    const GetFileMimeTypeCallback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE, {base::MayBlock()}, base::Bind(&GetMimeType, path), callback);
}
