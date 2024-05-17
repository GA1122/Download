void InputHandler::SynthesizeRepeatingScroll(
    SyntheticSmoothScrollGestureParams gesture_params,
    int repeat_count,
    base::TimeDelta repeat_delay,
    std::string interaction_marker_name,
    int id,
    std::unique_ptr<SynthesizeScrollGestureCallback> callback) {
  if (!interaction_marker_name.empty()) {
    TRACE_EVENT_COPY_ASYNC_BEGIN0("benchmark", interaction_marker_name.c_str(),
                                  id);
  }

  host_->GetRenderWidgetHost()->QueueSyntheticGesture(
      SyntheticGesture::Create(gesture_params),
      base::BindOnce(&InputHandler::OnScrollFinished,
                     weak_factory_.GetWeakPtr(), gesture_params, repeat_count,
                     repeat_delay, interaction_marker_name, id,
                     base::Passed(std::move(callback))));
}
