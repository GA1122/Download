void TemplateURL::CopyFrom(const TemplateURL& other) {
  if (this == &other)
    return;

  data_ = other.data_;
  ResizeURLRefVector();
  InvalidateCachedValues();
  SetPrepopulateId(other.data_.prepopulate_id);
}
