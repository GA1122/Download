WebContentDetectionResult RenderViewImpl::detectContentAround(
    const WebHitTestResult& touch_hit) {
  DCHECK(!touch_hit.isNull());
  DCHECK(!touch_hit.node().isNull());
  DCHECK(touch_hit.node().isTextNode());

  for (ContentDetectorList::const_iterator it = content_detectors_.begin();
      it != content_detectors_.end(); ++it) {
    ContentDetector::Result content = (*it)->FindTappedContent(touch_hit);
    if (content.valid) {
      return WebContentDetectionResult(content.content_boundaries,
          UTF8ToUTF16(content.text), content.intent_url);
    }
  }
  return WebContentDetectionResult();
}
