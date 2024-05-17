PaintPropertyTreeBuilderTest::DocScrollTranslation(const Document* document) {
  if (!document)
    document = &GetDocument();
  return document->GetLayoutView()
      ->FirstFragment()
      .PaintProperties()
      ->ScrollTranslation();
}
