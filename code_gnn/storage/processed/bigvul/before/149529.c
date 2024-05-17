void LoadingPredictor::MaybeAddPreconnect(
    const GURL& url,
    std::vector<PreconnectRequest> requests,
    HintOrigin origin) {
  DCHECK(!shutdown_);
  preconnect_manager()->Start(url, std::move(requests));
}
