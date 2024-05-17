bool ContentBrowserClient::CanCreateWindow(const GURL& opener_url,
                                           const GURL& origin,
                                           WindowContainerType container_type,
                                           ResourceContext* context,
                                           int render_process_id,
                                           bool* no_javascript_access) {
  *no_javascript_access = false;
  return true;
}
