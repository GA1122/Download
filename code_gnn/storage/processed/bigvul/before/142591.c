void WebstoreAPI::RemoveListeners(int routing_id,
                                  const std::string& extension_id,
                                  ObservedInstallInfoList* listeners) {
  for (ObservedInstallInfoList::iterator iter = listeners->begin();
       iter != listeners->end();) {
    if (iter->extension_id == extension_id && iter->routing_id == routing_id)
      iter = listeners->erase(iter);
    else
      ++iter;
  }
}
