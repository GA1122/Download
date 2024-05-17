    CompositedLayerRasterInvalidatorTest& Properties(
      const RefCountedPropertyTreeState& state) {
    data_.chunks.back().properties = state;
//       const TransformPaintPropertyNode& t,
//       const ClipPaintPropertyNode& c,
//       const EffectPaintPropertyNode& e) {
//     Properties(PropertyTreeState(&t, &c, &e));
      return *this;
    }