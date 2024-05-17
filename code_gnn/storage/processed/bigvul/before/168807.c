CSSPaintValue::CSSPaintValue(CSSCustomIdentValue* name)
    : CSSImageGeneratorValue(kPaintClass),
      name_(name),
      paint_image_generator_observer_(new Observer(this)) {}
