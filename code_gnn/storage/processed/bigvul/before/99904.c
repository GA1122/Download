NPError NPN_GetAuthenticationInfo(NPP id,
                                  const char* protocol,
                                  const char* host,
                                  int32_t port,
                                  const char* scheme,
                                  const char* realm,
                                  char** username,
                                  uint32* ulen,
                                  char** password,
                                  uint32* plen) {
  if (!id || !protocol || !host || !scheme || !realm || !username ||
      !ulen || !password || !plen)
    return NPERR_INVALID_PARAM;

  return NPERR_GENERIC_ERROR;
}
