static SkPath::FillType ParseWinding(const String& winding_rule_string) {
  if (winding_rule_string == "nonzero")
    return SkPath::kWinding_FillType;
  if (winding_rule_string == "evenodd")
    return SkPath::kEvenOdd_FillType;

  NOTREACHED();
  return SkPath::kEvenOdd_FillType;
}
