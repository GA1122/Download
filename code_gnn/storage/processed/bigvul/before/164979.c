static std::pair<blink::Image*, float> BrokenCanvas(float device_scale_factor) {
  if (device_scale_factor >= 2) {
    DEFINE_STATIC_REF(blink::Image, broken_canvas_hi_res,
                      (blink::Image::LoadPlatformResource("brokenCanvas@2x")));
    return std::make_pair(broken_canvas_hi_res, 2);
  }

  DEFINE_STATIC_REF(blink::Image, broken_canvas_lo_res,
                    (blink::Image::LoadPlatformResource("brokenCanvas")));
  return std::make_pair(broken_canvas_lo_res, 1);
}
