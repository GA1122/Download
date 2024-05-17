Dispatcher::Dispatcher(struct mg_context* context, const std::string& root)
    : context_(context), root_(root) {
  mg_set_uri_callback(context_, "/favicon.ico", &SendNoContentResponse, NULL);
}
