NPError NPN_PostURL(NPP id,
                    const char* url,
                    const char* target,
                    uint32 len,
                    const char* buf,
                    NPBool file) {

  return PostURLNotify(id, url, target, len, buf, file, false, 0);
}
