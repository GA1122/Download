const ScrollPaintPropertyNode* PaintPropertyTreeBuilderTest::DocScroll(
    const Document* document) {
  if (!document)
    document = &GetDocument();
  return document->GetLayoutView()->FirstFragment().PaintProperties()->Scroll();
}
