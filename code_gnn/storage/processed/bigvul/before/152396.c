  LinkRewritingDelegate(
      const std::map<GURL, base::FilePath>& url_to_local_path,
      const std::map<int, base::FilePath>& frame_routing_id_to_local_path)
      : url_to_local_path_(url_to_local_path),
        frame_routing_id_to_local_path_(frame_routing_id_to_local_path) {}
