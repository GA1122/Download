bool GetURLFromValue(const base::Value* in_value, std::string* out_value) {
  return in_value && out_value && in_value->GetAsString(out_value);
}
