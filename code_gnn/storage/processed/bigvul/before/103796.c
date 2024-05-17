void RenderView::LoadNavigationErrorPage(WebFrame* frame,
                                         const WebURLRequest& failed_request,
                                         const WebURLError& error,
                                         const std::string& html,
                                         bool replace) {
  std::string alt_html = !html.empty() ? html :
      content::GetContentClient()->renderer()->GetNavigationErrorHtml(
          failed_request, error);
  frame->loadHTMLString(alt_html,
                        GURL(chrome::kUnreachableWebDataURL),
                        error.unreachableURL,
                        replace);
}
