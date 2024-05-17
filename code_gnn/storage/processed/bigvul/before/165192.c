static IntPoint DragLocationForLink(const DragImage* link_image,
                                    const IntPoint& origin,
                                    float device_scale_factor,
                                    float page_scale_factor) {
  if (!link_image)
    return origin;

  FloatPoint image_offset(-link_image->Size().Width() / 2.f,
                          -kLinkDragBorderInset);
  float scale = 1.f / (device_scale_factor * page_scale_factor);
  image_offset.Scale(scale, scale);
  image_offset.MoveBy(origin);
  return RoundedIntPoint(image_offset);
}
