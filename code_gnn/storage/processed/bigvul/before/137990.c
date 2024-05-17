String AXLayoutObject::text() const {
  if (isPasswordFieldAndShouldHideValue()) {
    if (!getLayoutObject())
      return String();

    const ComputedStyle* style = getLayoutObject()->style();
    if (!style)
      return String();

    unsigned unmaskedTextLength = AXNodeObject::text().length();
    if (!unmaskedTextLength)
      return String();

    UChar maskCharacter = 0;
    switch (style->textSecurity()) {
      case TSNONE:
        break;   
      case TSDISC:
        maskCharacter = bulletCharacter;
        break;
      case TSCIRCLE:
        maskCharacter = whiteBulletCharacter;
        break;
      case TSSQUARE:
        maskCharacter = blackSquareCharacter;
        break;
    }
    if (maskCharacter) {
      StringBuilder maskedText;
      maskedText.reserveCapacity(unmaskedTextLength);
      for (unsigned i = 0; i < unmaskedTextLength; ++i)
        maskedText.append(maskCharacter);
      return maskedText.toString();
    }
  }

  return AXNodeObject::text();
}
