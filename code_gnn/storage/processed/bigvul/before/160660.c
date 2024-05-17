void RenderFrameImpl::OnFind(int request_id,
                             const base::string16& search_text,
                             const WebFindOptions& options) {
  DCHECK(!search_text.empty());

  blink::WebPlugin* plugin = GetWebPluginForFind();
  if (plugin) {
    if (options.find_next) {
      plugin->SelectFindResult(options.forward, request_id);
      render_view_->webview()->SetFocusedFrame(frame_);
    } else if (!plugin->StartFind(WebString::FromUTF16(search_text),
                                  options.match_case, request_id)) {
      SendFindReply(request_id, 0  , 0  ,
                    gfx::Rect(), true  );
    }
    return;
  }

  frame_->RequestFind(request_id, WebString::FromUTF16(search_text), options);
}
