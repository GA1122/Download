void LocalFrame::SetPageAndTextZoomFactors(float page_zoom_factor,
                                           float text_zoom_factor) {
  if (page_zoom_factor_ == page_zoom_factor &&
      text_zoom_factor_ == text_zoom_factor)
    return;

  Page* page = this->GetPage();
  if (!page)
    return;

  Document* document = this->GetDocument();
  if (!document)
    return;

  if (document->IsSVGDocument()) {
    if (!document->AccessSVGExtensions().ZoomAndPanEnabled())
      return;
  }

  page_zoom_factor_ = page_zoom_factor;
  text_zoom_factor_ = text_zoom_factor;

  for (Frame* child = Tree().FirstChild(); child;
       child = child->Tree().NextSibling()) {
    if (child->IsLocalFrame())
      ToLocalFrame(child)->SetPageAndTextZoomFactors(page_zoom_factor_,
                                                     text_zoom_factor_);
  }

  document->MediaQueryAffectingValueChanged();
  document->SetNeedsStyleRecalc(
      kSubtreeStyleChange,
      StyleChangeReasonForTracing::Create(StyleChangeReason::kZoom));
  document->UpdateStyleAndLayoutIgnorePendingStylesheets();
}
