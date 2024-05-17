  virtual void DidFinishLoading() {
    String result;
    bool base64_encoded;
    if (InspectorPageAgent::SharedBufferContent(raw_data_, mime_type_,
                                                text_encoding_name_, &result,
                                                &base64_encoded))
      callback_->sendSuccess(result, base64_encoded);
    else
      callback_->sendFailure(Response::Error("Couldn't encode data"));
    Dispose();
  }
