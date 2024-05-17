base::ThreadLocalPointer<MessageLoop>* GetTLSMessageLoop() {
  static auto* lazy_tls_ptr = new base::ThreadLocalPointer<MessageLoop>();
  return lazy_tls_ptr;
}
