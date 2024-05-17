  content::DownloadFile* CreateFile(
      DownloadCreateInfo* info,
      scoped_ptr<content::ByteStreamReader> stream_reader,
      DownloadManager* mgr,
      bool calculate_hash,
      const net::BoundNetLog& bound_net_log) {
    return MockCreateFile(
        info, stream_reader.get(), info->request_handle, mgr, calculate_hash,
        bound_net_log);
  }
