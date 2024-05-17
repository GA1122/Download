PP_InputEvent GetPPEventWithCommonFieldsAndType(
    const WebInputEvent& web_event) {
  PP_InputEvent result;
  memset(&result, 0, sizeof(PP_InputEvent));
  result.type = ConvertEventTypes(web_event.type);
  result.time_stamp = web_event.timeStampSeconds;
  return result;
}
