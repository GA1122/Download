void Dispatcher::ForbidAllOtherRequests() {
  mg_set_uri_callback(context_, "*", &SendForbidden, NULL);
}
