void Document::DidAssociateFormControlsTimerFired(TimerBase* timer) {
  DCHECK_EQ(timer, &did_associate_form_controls_timer_);
  if (!GetFrame() || !GetFrame()->GetPage())
    return;

  GetFrame()->GetPage()->GetChromeClient().DidAssociateFormControlsAfterLoad(
      GetFrame());
}
