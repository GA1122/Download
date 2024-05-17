  Element* GetElementById(const AtomicString& id) {
    WebLocalFrameImpl* frame = web_view_helper_->LocalMainFrame();
    return frame->GetFrame()->GetDocument()->getElementById(id);
  }
