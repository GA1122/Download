void RenderViewImpl::OnFind(int request_id,
                            const string16& search_text,
                            const WebFindOptions& options) {
#if defined(OS_ANDROID)
  if (synchronous_find_reply_message_.get())
    return;
#endif
  Find(request_id, search_text, options);
}
