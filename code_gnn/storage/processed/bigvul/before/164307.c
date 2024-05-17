void AssignOptionalValue(const std::unique_ptr<T>& source,
                         std::unique_ptr<T>* destination) {
  if (source)
    *destination = std::make_unique<T>(*source);
}
