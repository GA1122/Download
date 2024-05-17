  static v8::Handle<v8::Value> IsIncognitoProcess(const v8::Arguments& args) {
    return v8::Boolean::New(
        ChromeRenderProcessObserver::is_incognito_process());
  }
