ResourceDispatcherHostImpl::GetTrafficAnnotation() {
  return net::DefineNetworkTrafficAnnotation("resource_dispatcher_host",
                                             R"(
        semantics {
          sender: "Resource Dispatcher Host"
          description:
            "Navigation-initiated request or renderer process initiated "
            "request, which includes all resources for normal page loads, "
            "chrome URLs, resources for installed extensions, as well as "
            "downloads."
          trigger:
            "Navigating to a URL or downloading a file. A webpage, "
            "ServiceWorker, chrome: 
            "requests in the background."
          data: "Anything the initiator wants to send."
          destination: OTHER
        }
        policy {
          cookies_allowed: YES
          cookies_store: "user or per-app cookie store"
          setting: "These requests cannot be disabled."
          policy_exception_justification:
            "Not implemented. Without these requests, Chrome will be unable to "
            "load any webpage."
        })");
}
