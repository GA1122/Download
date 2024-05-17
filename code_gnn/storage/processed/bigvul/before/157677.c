ExtensionFunction::ResponseAction InputImeSetCompositionFunction::Run() {
  InputImeEventRouter* event_router =
      GetInputImeEventRouter(Profile::FromBrowserContext(browser_context()));
  InputMethodEngineBase* engine =
      event_router ? event_router->GetActiveEngine(extension_id()) : nullptr;
  if (engine) {
    std::unique_ptr<SetComposition::Params> parent_params(
        SetComposition::Params::Create(*args_));
    const SetComposition::Params::Parameters& params =
        parent_params->parameters;
    std::vector<InputMethodEngineBase::SegmentInfo> segments;
    if (params.segments) {
      for (const auto& segments_arg : *params.segments) {
        EXTENSION_FUNCTION_VALIDATE(segments_arg.style !=
                                    input_ime::UNDERLINE_STYLE_NONE);
        InputMethodEngineBase::SegmentInfo segment_info;
        segment_info.start = segments_arg.start;
        segment_info.end = segments_arg.end;
        if (segments_arg.style == input_ime::UNDERLINE_STYLE_UNDERLINE) {
          segment_info.style = InputMethodEngineBase::SEGMENT_STYLE_UNDERLINE;
        } else if (segments_arg.style ==
                   input_ime::UNDERLINE_STYLE_DOUBLEUNDERLINE) {
          segment_info.style =
              InputMethodEngineBase::SEGMENT_STYLE_DOUBLE_UNDERLINE;
        } else {
          segment_info.style =
              InputMethodEngineBase::SEGMENT_STYLE_NO_UNDERLINE;
        }
        segments.push_back(segment_info);
      }
    }
    int selection_start =
        params.selection_start ? *params.selection_start : params.cursor;
    int selection_end =
        params.selection_end ? *params.selection_end : params.cursor;
    std::string error;
    if (!engine->SetComposition(params.context_id, params.text.c_str(),
                                selection_start, selection_end, params.cursor,
                                segments, &error)) {
      std::unique_ptr<base::ListValue> results =
          std::make_unique<base::ListValue>();
      results->Append(std::make_unique<base::Value>(false));
      return RespondNow(ErrorWithArguments(std::move(results), error));
    }
  }
  return RespondNow(OneArgument(std::make_unique<base::Value>(true)));
}
