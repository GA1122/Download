bool LocalFrame::ShouldUsePrintingLayout() const {
  return GetDocument()->Printing() &&
         (!Tree().Parent() || !Tree().Parent()->IsLocalFrame() ||
          !ToLocalFrame(Tree().Parent())->GetDocument()->Printing());
}
