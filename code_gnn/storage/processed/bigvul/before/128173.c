  void OnPpapiClearSiteDataResult(uint32 request_id, bool success) {
    DCHECK_EQ(0u, request_id);
    OnClearSiteDataResult(success);
  }
