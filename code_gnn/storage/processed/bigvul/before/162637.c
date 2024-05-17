HeadlessPrintManager::HeadlessPrintManager(content::WebContents* web_contents)
    : PrintManager(web_contents) {
  Reset();
}
