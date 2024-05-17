Touch* Document::createTouch(DOMWindow* window,
                             EventTarget* target,
                             int identifier,
                             double page_x,
                             double page_y,
                             double screen_x,
                             double screen_y,
                             double radius_x,
                             double radius_y,
                             float rotation_angle,
                             float force) const {
  if (!std::isfinite(page_x))
    page_x = 0;
  if (!std::isfinite(page_y))
    page_y = 0;
  if (!std::isfinite(screen_x))
    screen_x = 0;
  if (!std::isfinite(screen_y))
    screen_y = 0;
  if (!std::isfinite(radius_x))
    radius_x = 0;
  if (!std::isfinite(radius_y))
    radius_y = 0;
  if (!std::isfinite(rotation_angle))
    rotation_angle = 0;
  if (!std::isfinite(force))
    force = 0;

  if (radius_x || radius_y || rotation_angle || force) {
    UseCounter::Count(*this,
                      WebFeature::kDocumentCreateTouchMoreThanSevenArguments);
  }

  LocalFrame* frame = window && window->IsLocalDOMWindow()
                          ? blink::ToLocalDOMWindow(window)->GetFrame()
                          : GetFrame();
  return Touch::Create(
      frame, target, identifier, FloatPoint(screen_x, screen_y),
      FloatPoint(page_x, page_y), FloatSize(radius_x, radius_y), rotation_angle,
      force, String());
}
