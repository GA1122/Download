void WebContext::DeleteCookiesCallback(int request_id, int num_deleted) {
  client_->CookiesDeleted(request_id, num_deleted);
}
