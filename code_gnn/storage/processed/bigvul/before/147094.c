void WebLocalFrameImpl::SetTickmarks(const WebVector<WebRect>& tickmarks) {
  if (GetFrameView()) {
    Vector<IntRect> tickmarks_converted(tickmarks.size());
    for (size_t i = 0; i < tickmarks.size(); ++i)
      tickmarks_converted[i] = tickmarks[i];
    GetFrameView()->SetTickmarks(tickmarks_converted);
  }
}
