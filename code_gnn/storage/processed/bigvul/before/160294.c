ImageLoader::DecodeRequest::DecodeRequest(ImageLoader* loader,
                                          ScriptPromiseResolver* resolver)
    : request_id_(s_next_request_id_++), resolver_(resolver), loader_(loader) {}
