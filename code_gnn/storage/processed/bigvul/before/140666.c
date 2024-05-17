void OutOfProcessInstance::SubmitForm(const std::string& url,
                                      const void* data,
                                      int length) {
  pp::URLRequestInfo request(this);
  request.SetURL(url);
  request.SetMethod("POST");
  request.AppendDataToBody(reinterpret_cast<const char*>(data), length);

  pp::CompletionCallback callback =
      form_factory_.NewCallback(&OutOfProcessInstance::FormDidOpen);
  form_loader_ = CreateURLLoaderInternal();
  int rv = form_loader_.Open(request, callback);
  if (rv != PP_OK_COMPLETIONPENDING)
    callback.Run(rv);
}
