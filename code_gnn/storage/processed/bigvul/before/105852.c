void UrlmonUrlRequest::TerminateTransaction() {
  if (cleanup_transaction_ && bind_context_ && moniker_) {
    static const int kUrlmonTerminateTransactionFlags = 0x2000000;
    base::win::ScopedComPtr<BindContextInfo> info;
    BindContextInfo::FromBindContext(bind_context_, info.Receive());
    DCHECK(info);
    if (info && info->protocol()) {
      info->protocol()->Terminate(kUrlmonTerminateTransactionFlags);
    }
  }
  bind_context_.Release();
}
