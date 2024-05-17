Decimal InputType::ParseToNumberOrNaN(const String& string) const {
  return ParseToNumber(string, Decimal::Nan());
}
