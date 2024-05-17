pp::URLLoader OutOfProcessInstance::CreateURLLoader() {
  if (full_) {
    if (!did_call_start_loading_) {
      did_call_start_loading_ = true;
      pp::PDF::DidStartLoading(this);
    }

    pp::PDF::SetContentRestriction(this, CONTENT_RESTRICTION_SAVE |
                                   CONTENT_RESTRICTION_PRINT);
  }

  return CreateURLLoaderInternal();
}
