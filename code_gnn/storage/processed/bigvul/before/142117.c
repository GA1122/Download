void GetURLAndTitleToBookmark(content::WebContents* web_contents,
                              GURL* url,
                              base::string16* title) {
  *url = GetURLToBookmark(web_contents);
  *title = web_contents->GetTitle();
}
