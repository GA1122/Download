void FileSystemManagerImpl::ResolveURL(const GURL& filesystem_url,
                                       ResolveURLCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(filesystem_url));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(blink::mojom::FileSystemInfo::New(),
                            base::FilePath(), false, opt_error.value());
    return;
  }

  if (!security_policy_->CanReadFileSystemFile(process_id_, url)) {
    std::move(callback).Run(blink::mojom::FileSystemInfo::New(),
                            base::FilePath(), false,
                            base::File::FILE_ERROR_SECURITY);
    return;
  }

  context_->ResolveURL(
      url, base::BindOnce(&FileSystemManagerImpl::DidResolveURL, GetWeakPtr(),
                          std::move(callback)));
}
