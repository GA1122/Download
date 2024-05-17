  MHTMLPartsGenerationDelegate(
      const mojom::SerializeAsMHTMLParams& params,
      std::unordered_set<std::string>* serialized_resources_uri_digests)
      : params_(params),
        serialized_resources_uri_digests_(serialized_resources_uri_digests) {
    DCHECK(serialized_resources_uri_digests_);
    DCHECK(std::is_sorted(params_.digests_of_uris_to_skip.begin(),
                          params_.digests_of_uris_to_skip.end()));
    DCHECK(std::adjacent_find(params_.digests_of_uris_to_skip.begin(),
                              params_.digests_of_uris_to_skip.end()) ==
           params_.digests_of_uris_to_skip.end());
  }
