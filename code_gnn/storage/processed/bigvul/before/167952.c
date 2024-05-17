LocalFrame& LocalFrame::LocalFrameRoot() const {
  const LocalFrame* cur_frame = this;
  while (cur_frame && cur_frame->Tree().Parent() &&
         cur_frame->Tree().Parent()->IsLocalFrame())
    cur_frame = ToLocalFrame(cur_frame->Tree().Parent());

  return const_cast<LocalFrame&>(*cur_frame);
}
