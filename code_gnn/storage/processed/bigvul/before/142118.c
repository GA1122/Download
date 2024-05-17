GURL GetURLToBookmark(content::WebContents* web_contents) {
  DCHECK(web_contents);
  return search::IsInstantNTP(web_contents) ? GURL(kChromeUINewTabURL)
                                            : web_contents->GetURL();
}
