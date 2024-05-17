void InspectorNetworkAgent::DidFailXHRLoading(ExecutionContext* context,
                                              XMLHttpRequest* xhr,
                                              ThreadableLoaderClient* client,
                                              const AtomicString& method,
                                              const String& url) {
  DidFinishXHRInternal(context, xhr, client, method, url, false);
}
