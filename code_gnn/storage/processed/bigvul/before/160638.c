  MHTMLPartsGenerationDelegate(
      const FrameMsg_SerializeAsMHTML_Params& params,
      std::set<std::string>* serialized_resources_uri_digests)
      : params_(params),
        serialized_resources_uri_digests_(serialized_resources_uri_digests) {
    DCHECK(serialized_resources_uri_digests_);
  }
