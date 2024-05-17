void Histogram::GetParameters(DictionaryValue* params) const {
  params->SetString("type", HistogramTypeToString(GetHistogramType()));
  params->SetInteger("min", declared_min());
  params->SetInteger("max", declared_max());
  params->SetInteger("bucket_count", static_cast<int>(bucket_count()));
}
