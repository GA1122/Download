void ServiceWorkerContextCore::HasMainFrameProviderHost(
    const GURL& origin,
    BoolCallback callback) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  ProviderHostIterator provider_host_iterator(
      providers_.get(),
      base::BindRepeating(IsSameOriginWindowProviderHost, origin));

  if (provider_host_iterator.IsAtEnd()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback), false));
    return;
  }

  std::unique_ptr<std::vector<std::pair<int, int>>> render_frames(
      new std::vector<std::pair<int, int>>());

  while (!provider_host_iterator.IsAtEnd()) {
    ServiceWorkerProviderHost* provider_host =
        provider_host_iterator.GetProviderHost();
    render_frames->push_back(
        std::make_pair(provider_host->process_id(), provider_host->frame_id()));
    provider_host_iterator.Advance();
  }

  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&FrameListContainsMainFrameOnUI, std::move(render_frames)),
      std::move(callback));
}
