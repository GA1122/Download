void Browser::RegisterIntentHandler(
    WebContents* web_contents,
    const webkit_glue::WebIntentServiceData& data,
    bool user_gesture) {
  RegisterIntentHandlerHelper(web_contents, data, user_gesture);
}
