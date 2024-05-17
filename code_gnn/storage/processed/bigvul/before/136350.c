PaintPropertyTreeBuilderTest::PaintPropertiesForElement(const char* name) {
  return GetDocument()
      .getElementById(name)
      ->GetLayoutObject()
      ->FirstFragment()
      .PaintProperties();
}
