void ChromeContentBrowserClient::AllowCertificateError(
    SSLCertErrorHandler* handler,
    bool overridable,
    Callback2<SSLCertErrorHandler*, bool>::Type* callback) {
  TabContents* tab = tab_util::GetTabContentsByID(
      handler->render_process_host_id(),
      handler->tab_contents_id());
  if (!tab) {
    NOTREACHED();
    return;
  }
  prerender::PrerenderManager* prerender_manager =
      tab->profile()->GetPrerenderManager();
  if (prerender_manager && prerender_manager->IsTabContentsPrerendering(tab)) {
    if (prerender_manager->prerender_tracker()->TryCancel(
            handler->render_process_host_id(),
            handler->tab_contents_id(),
            prerender::FINAL_STATUS_SSL_ERROR)) {
      handler->CancelRequest();
      return;
    }
  }

  SSLBlockingPage* blocking_page = new SSLBlockingPage(
      handler, overridable, callback);
  blocking_page->Show();
}
