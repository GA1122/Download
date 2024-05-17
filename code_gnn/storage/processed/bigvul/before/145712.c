void ApiTestEnvironment::InitializeEnvironment() {
  gin::Dictionary global(env()->isolate(),
                         env()->context()->v8_context()->Global());
  gin::Dictionary navigator(gin::Dictionary::CreateEmpty(env()->isolate()));
  navigator.Set("appVersion", base::StringPiece(""));
  global.Set("navigator", navigator);
  gin::Dictionary chrome(gin::Dictionary::CreateEmpty(env()->isolate()));
  global.Set("chrome", chrome);
  gin::Dictionary extension(gin::Dictionary::CreateEmpty(env()->isolate()));
  chrome.Set("extension", extension);
  gin::Dictionary runtime(gin::Dictionary::CreateEmpty(env()->isolate()));
  chrome.Set("runtime", runtime);
}
