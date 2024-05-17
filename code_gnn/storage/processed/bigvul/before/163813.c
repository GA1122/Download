void OnHttpHeaderReceived(const std::string& header,
                          const std::string& value,
                          int child_id,
                          content::ResourceContext* resource_context,
                          content::OnHeaderProcessedCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  GURL origin(value);
  DCHECK(origin.SchemeIs(extensions::kExtensionScheme));

  callback.Run(CheckOriginHeader(resource_context, child_id, origin));
}
