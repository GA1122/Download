NPError NPN_GetURLNotify(NPP id,
                         const char* url,
                         const char* target,
                         void* notify_data) {



  return GetURLNotify(id, url, target, true, notify_data);
}
