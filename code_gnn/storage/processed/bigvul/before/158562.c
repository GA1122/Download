void WebLocalFrameImpl::PostPausableTask(PausableTaskCallback callback) {
  DCHECK(GetFrame());
  Document* document = GetFrame()->GetDocument();
  DCHECK(document);
  PausableTask::Post(document, std::move(callback));
}
