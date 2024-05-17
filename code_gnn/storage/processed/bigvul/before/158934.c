int PDFiumEngine::Form_SetTimer(FPDF_FORMFILLINFO* param,
                                int elapse,
                                TimerCallback timer_func) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  base::TimeDelta elapse_time = base::TimeDelta::FromMilliseconds(elapse);
  engine->formfill_timers_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(++engine->next_formfill_timer_id_),
      std::forward_as_tuple(elapse_time, timer_func));
  engine->client_->ScheduleCallback(engine->next_formfill_timer_id_,
                                    elapse_time);
  return engine->next_formfill_timer_id_;
}
