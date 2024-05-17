static void LocalToPageQuad(const LayoutObject& layout_object,
                            const LayoutRect& rect,
                            FloatQuad* quad) {
  LocalFrame* frame = layout_object.GetFrame();
  LocalFrameView* view = frame->View();
  FloatQuad absolute =
      layout_object.LocalToAbsoluteQuad(FloatQuad(FloatRect(rect)));
  quad->SetP1(view->ContentsToRootFrame(RoundedIntPoint(absolute.P1())));
  quad->SetP2(view->ContentsToRootFrame(RoundedIntPoint(absolute.P2())));
  quad->SetP3(view->ContentsToRootFrame(RoundedIntPoint(absolute.P3())));
  quad->SetP4(view->ContentsToRootFrame(RoundedIntPoint(absolute.P4())));
}
