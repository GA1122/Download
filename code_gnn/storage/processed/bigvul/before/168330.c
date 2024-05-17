void BrowserView::OnWidgetDestroying(views::Widget* widget) {
  std::vector<std::unique_ptr<content::WebContents>> contents;
  while (browser()->tab_strip_model()->count())
    contents.push_back(browser()->tab_strip_model()->DetachWebContentsAt(0));
  for (auto& content : contents)
    content.reset();
}
