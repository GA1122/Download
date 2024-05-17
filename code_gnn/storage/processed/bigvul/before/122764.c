void BrowserPluginGuest::RunFileChooser(WebContents* web_contents,
                                        const FileChooserParams& params) {
  embedder_web_contents_->GetDelegate()->RunFileChooser(web_contents, params);
}
