AwContents* AwContents::FromWebContents(WebContents* web_contents) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  return AwContentsUserData::GetContents(web_contents);
}
