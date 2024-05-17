ChromeDownloadManagerDelegate::GetDownloadIdReceiverCallback() {
  return base::Bind(&ChromeDownloadManagerDelegate::SetNextId,
                    weak_ptr_factory_.GetWeakPtr());
}
