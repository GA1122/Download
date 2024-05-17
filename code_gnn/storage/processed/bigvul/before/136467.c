void PrintTo(const SkRect& rect, std::ostream* os) {
  *os << (cc::PaintOp::IsUnsetRect(rect) ? "(unset)"
                                         : blink::FloatRect(rect).ToString());
}
