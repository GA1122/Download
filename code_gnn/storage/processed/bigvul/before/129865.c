void TraceEventTestFixture::OnTraceDataCollected(
    WaitableEvent* flush_complete_event,
    const scoped_refptr<base::RefCountedString>& events_str,
    bool has_more_events) {
  num_flush_callbacks_++;
  if (num_flush_callbacks_ > 1) {
    EXPECT_FALSE(events_str->data().empty());
  }
  AutoLock lock(lock_);
  json_output_.json_output.clear();
  trace_buffer_.Start();
  trace_buffer_.AddFragment(events_str->data());
  trace_buffer_.Finish();

  scoped_ptr<Value> root = base::JSONReader::Read(
      json_output_.json_output, JSON_PARSE_RFC | JSON_DETACHABLE_CHILDREN);

  if (!root.get()) {
    LOG(ERROR) << json_output_.json_output;
  }

  ListValue* root_list = NULL;
  ASSERT_TRUE(root.get());
  ASSERT_TRUE(root->GetAsList(&root_list));

  while (root_list->GetSize()) {
    scoped_ptr<Value> item;
    root_list->Remove(0, &item);
    trace_parsed_.Append(item.release());
  }

  if (!has_more_events)
    flush_complete_event->Signal();
}
