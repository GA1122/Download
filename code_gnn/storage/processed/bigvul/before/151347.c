void FillCommonFrameData(TracedValue* frame_data, LocalFrame* frame) {
  frame_data->SetString("frame", ToHexString(frame));
  frame_data->SetString("url", UrlForFrame(frame));
  frame_data->SetString("name", frame->Tree().GetName());

  FrameOwner* owner = frame->Owner();
  if (owner && owner->IsLocal()) {
    frame_data->SetInteger(
        "nodeId", DOMNodeIds::IdForNode(ToHTMLFrameOwnerElement(owner)));
  }
  Frame* parent = frame->Tree().Parent();
  if (parent && parent->IsLocalFrame())
    frame_data->SetString("parent", ToHexString(parent));
}
