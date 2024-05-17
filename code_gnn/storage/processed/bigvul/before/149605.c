std::unique_ptr<ResolveHostClientImpl> PreconnectManager::PreresolveUrl(
    const GURL& url,
    ResolveHostCallback callback) const {
  DCHECK(url.GetOrigin() == url);
  DCHECK(url.SchemeIsHTTPOrHTTPS());

  auto* network_context = GetNetworkContext();
  if (!network_context) {
    base::PostTask(
        FROM_HERE,
        {content::BrowserThread::UI, content::BrowserTaskType::kPreconnect},
        base::BindOnce(std::move(callback), false));
    return nullptr;
  }

  return std::make_unique<ResolveHostClientImpl>(url, std::move(callback),
                                                 network_context);
}
