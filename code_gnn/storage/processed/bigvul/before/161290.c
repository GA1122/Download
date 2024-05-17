bool StringToGestureSourceType(Maybe<std::string> in,
                               SyntheticGestureParams::GestureSourceType& out) {
  if (!in.isJust()) {
    out = SyntheticGestureParams::GestureSourceType::DEFAULT_INPUT;
    return true;
  }
  if (in.fromJust() == Input::GestureSourceTypeEnum::Default) {
    out = SyntheticGestureParams::GestureSourceType::DEFAULT_INPUT;
    return true;
  }
  if (in.fromJust() == Input::GestureSourceTypeEnum::Touch) {
    out = SyntheticGestureParams::GestureSourceType::TOUCH_INPUT;
    return true;
  }
  if (in.fromJust() == Input::GestureSourceTypeEnum::Mouse) {
    out = SyntheticGestureParams::GestureSourceType::MOUSE_INPUT;
    return true;
  }
  return false;
}
