void ChromeClientImpl::RequestDecode(LocalFrame* frame,
                                     const PaintImage& image,
                                     base::OnceCallback<void(bool)> callback) {
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  web_frame->LocalRoot()->FrameWidget()->RequestDecode(image,
                                                       std::move(callback));
}