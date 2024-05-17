std::string BlinkTestRunner::DumpHistoryForWindow(WebTestProxyBase* proxy) {
  size_t pos = 0;
  std::vector<int>::iterator id;
  for (id = routing_ids_.begin(); id != routing_ids_.end(); ++id, ++pos) {
    RenderView* render_view = RenderView::FromRoutingID(*id);
    if (!render_view) {
      NOTREACHED();
      continue;
    }
    if (BlinkTestRunner::Get(render_view)->proxy() == proxy)
      break;
  }

  if (id == routing_ids_.end()) {
    NOTREACHED();
    return std::string();
  }
  return DumpBackForwardList(session_histories_[pos],
                             current_entry_indexes_[pos]);
}
