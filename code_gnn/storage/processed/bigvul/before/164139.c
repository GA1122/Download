  int GetResponseInfoSize(const net::HttpResponseInfo* info) {
    base::Pickle pickle;
    return PickleResponseInfo(&pickle, info);
  }
