void AssignOptionalValue(const std::unique_ptr<T>& source,
                         std::unique_ptr<T>& destination) {
  if (source.get()) {
    destination.reset(new T(*source));
  }
}
