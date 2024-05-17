HttpBridge::HttpBridge(HttpBridge::RequestContextGetter* context_getter)
    : context_getter_for_request_(context_getter),
      created_on_loop_(MessageLoop::current()),
      http_post_completed_(false, false) {
}
