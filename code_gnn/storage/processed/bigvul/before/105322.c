  virtual void OnUploadedAutofillHeuristics(const std::string& form_signature) {
    ResponseData response;
    response.type_of_response = UPLOAD_SUCCESSFULL;
    responses_.push_back(response);
  }
