bool PrintWebViewHelper::CopyAndPrint(WebKit::WebFrame* web_frame) {
  webkit_glue::WebPreferences prefs = render_view()->GetWebkitPreferences();
  prefs.javascript_enabled = false;
  prefs.java_enabled = false;

  print_web_view_ = WebKit::WebView::create(this);
  prefs.Apply(print_web_view_);
  print_web_view_->initializeMainFrame(this);

  print_pages_params_->pages.clear();   

  std::string html = web_frame->selectionAsMarkup().utf8();
  std::string url_str = "data:text/html;charset=utf-8,";
  url_str.append(html);
  GURL url(url_str);

  print_web_view_->mainFrame()->loadRequest(WebKit::WebURLRequest(url));

  return true;
}