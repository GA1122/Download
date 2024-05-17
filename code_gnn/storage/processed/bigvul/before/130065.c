void VariationsService::SetRestrictMode(const std::string& restrict_mode) {
  DCHECK(thread_checker_.CalledOnValidThread());

  DCHECK(variations_server_url_.is_empty());
  restrict_mode_ = restrict_mode;
}
