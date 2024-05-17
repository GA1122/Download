WebContentsDestroyedObserver::WebContentsDestroyedObserver(
    WebContents* web_contents)
    : WebContentsObserver(web_contents) {
  DCHECK(web_contents);
}
