void PageCaptureSaveAsMHTMLFunction::ReturnSuccess(int64_t file_size) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  WebContents* web_contents = GetWebContents();
  if (!web_contents || !render_frame_host()) {
    ReturnFailure(kTabClosedError);
    return;
  }

  int child_id = render_frame_host()->GetProcess()->GetID();
  ChildProcessSecurityPolicy::GetInstance()->GrantReadFile(
      child_id, mhtml_path_);

  std::unique_ptr<base::DictionaryValue> dict(new base::DictionaryValue());
  dict->SetString("mhtmlFilePath", mhtml_path_.value());
  dict->SetInteger("mhtmlFileLength", file_size);
  SetResult(std::move(dict));

  SendResponse(true);

}
