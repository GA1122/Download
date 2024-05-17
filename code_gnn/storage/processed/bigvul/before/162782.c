void BaseRenderingContext2D::strokeStyle(
    StringOrCanvasGradientOrCanvasPattern& return_value) const {
  ConvertCanvasStyleToUnionType(GetState().StrokeStyle(), return_value);
}
