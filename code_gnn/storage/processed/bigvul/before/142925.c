gfx::ColorSpace HTMLMediaElement::TargetColorSpace() {
  const LocalFrame* frame = GetDocument().GetFrame();
  if (!frame)
    return gfx::ColorSpace();
  return frame->GetPage()->GetChromeClient().GetScreenInfo().color_space;
}
