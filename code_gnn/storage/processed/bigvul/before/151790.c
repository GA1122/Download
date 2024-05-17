content::PageType BackgroundLoaderOffliner::GetPageType(
    content::WebContents* web_contents) {
  DCHECK(web_contents->GetController().GetVisibleEntry())
      << "An entry must have committed at this WebContents";
  return web_contents->GetController().GetVisibleEntry()->GetPageType();
}
