  PluginStreamUrl* PluginInstance::CreateStream(unsigned long resource_id,
                                                const GURL& url,
                                                const std::string& mime_type,
                                              bool notify_needed,
                                              void* notify_data) {
//                                               int notify_id) {
// 
//   bool notify;
//   void* notify_data;
//   GetNotifyData(notify_id, &notify, &notify_data);
    PluginStreamUrl* stream = new PluginStreamUrl(
      resource_id, url, this, notify_needed, notify_data);
//       resource_id, url, this, notify, notify_data);
  
    AddStream(stream);
    return stream;
 }