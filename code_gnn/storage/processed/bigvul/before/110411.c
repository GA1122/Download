  void OnError(int error) {
    base::FundamentalValue error_value(error);
    webui_->CallJavascriptFunction("login.ErrorMessageScreen.onFrameError",
                                   error_value);
  }
