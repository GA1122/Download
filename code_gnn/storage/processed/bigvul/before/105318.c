  void InitContextGetter() {
    Profile::set_default_request_context(request_context_getter.get());
  }
