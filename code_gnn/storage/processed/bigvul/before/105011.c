void HttpBridge::SetPostPayload(const char* content_type,
                                int content_length,
                                const char* content) {
  DCHECK_EQ(MessageLoop::current(), created_on_loop_);
  if (DCHECK_IS_ON()) {
    base::AutoLock lock(fetch_state_lock_);
    DCHECK(!fetch_state_.request_completed);
  }
  DCHECK(content_type_.empty()) << "Bridge payload already set.";
  DCHECK_GE(content_length, 0) << "Content length < 0";
  content_type_ = content_type;
  if (!content || (content_length == 0)) {
    DCHECK_EQ(content_length, 0);
    request_content_ = " ";   
  } else {
    request_content_.assign(content, content_length);
  }
}
