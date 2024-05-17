T ValueOrDefault(Optional&& opt) {
  return std::forward<Optional>(opt).value_or(T{});
}
