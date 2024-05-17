void ResourceDispatcherHostImpl::RegisterInterceptor(
    const std::string& http_header,
    const std::string& starts_with,
    const InterceptorCallback& interceptor) {
  DCHECK(!http_header.empty());
  DCHECK(interceptor);
  DCHECK(http_header_interceptor_map_.find(http_header) ==
         http_header_interceptor_map_.end());

  HeaderInterceptorInfo interceptor_info;
  interceptor_info.starts_with = starts_with;
  interceptor_info.interceptor = interceptor;

  http_header_interceptor_map_[http_header] = interceptor_info;
}
