base::ThreadLocalStorage::Slot& DangerousPatternTLS() {
  static base::NoDestructor<base::ThreadLocalStorage::Slot>
      dangerous_pattern_tls(&OnThreadTermination);
  return *dangerous_pattern_tls;
}
