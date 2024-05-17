  void PluginInstance::SendJavaScriptStream(const GURL& url,
                                            const std::string& result,
                                            bool success,
                                          bool notify_needed,
                                          intptr_t notify_data) {
//                                           int notify_id) {
//   bool notify;
//   void* notify_data;
//   GetNotifyData(notify_id, &notify, &notify_data);
// 
    if (success) {
      PluginStringStream *stream =
      new PluginStringStream(this, url, notify_needed,
                             reinterpret_cast<void*>(notify_data));
//         new PluginStringStream(this, url, notify, notify_data);
      AddStream(stream);
      stream->SendToPlugin(result, "text/html");
    } else {
    if (notify_needed) {
      this->NPP_URLNotify(url.spec().c_str(), NPRES_DONE,
                          reinterpret_cast<void*>(notify_data));
    }
//     if (notify)
//       NPP_URLNotify(url.spec().c_str(), NPRES_DONE, notify_data);
    }
  }