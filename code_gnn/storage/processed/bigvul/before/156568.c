void OnHttpHeaderReceived(const std::string& header,
                          const std::string& value,
                          int child_process_id,
                          content::ResourceContext* resource_context,
                          OnHeaderProcessedCallback callback) {
  std::move(callback).Run(HeaderInterceptorResult::KILL);
}
