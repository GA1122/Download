network::ResourceRequest CreateXHRRequest(const char* url) {
  network::ResourceRequest request;
  request.method = "GET";
  request.url = GURL(url);
  request.referrer_policy = Referrer::GetDefaultReferrerPolicy();
  request.request_initiator = url::Origin();
  request.load_flags = 0;
  request.plugin_child_id = -1;
  request.resource_type = RESOURCE_TYPE_XHR;
  request.appcache_host_id = kAppCacheNoHostId;
  request.should_reset_appcache = false;
  request.is_main_frame = true;
  request.transition_type = ui::PAGE_TRANSITION_LINK;
  request.allow_download = true;
  return request;
}
