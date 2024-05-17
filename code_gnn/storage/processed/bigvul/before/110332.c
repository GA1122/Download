  ProgressEvent(const char* event_type,
                const nacl::string& url,
                Plugin::LengthComputable length_computable,
                uint64_t loaded_bytes,
                uint64_t total_bytes) :
    event_type_(event_type),
    url_(url),
    length_computable_(length_computable),
    loaded_bytes_(loaded_bytes),
    total_bytes_(total_bytes) { }
