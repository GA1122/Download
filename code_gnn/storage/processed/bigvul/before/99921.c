NPError NPN_PostURLNotify(NPP id,
                          const char* url,
                          const char* target,
                          uint32 len,
                          const char* buf,
                          NPBool file,
                          void* notify_data) {
  return PostURLNotify(id, url, target, len, buf, file, true, notify_data);
}
