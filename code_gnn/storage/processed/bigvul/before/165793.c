bool SVGElement::HasTransform(
    ApplyMotionTransform apply_motion_transform) const {
  return (GetLayoutObject() && GetLayoutObject()->StyleRef().HasTransform()) ||
         (apply_motion_transform == kIncludeMotionTransform &&
          HasSVGRareData());
}
