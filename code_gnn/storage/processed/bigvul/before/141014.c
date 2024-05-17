void Document::DidAssociateFormControl(Element* element) {
  if (!GetFrame() || !GetFrame()->GetPage() || !HasFinishedParsing())
    return;

  if (!did_associate_form_controls_timer_.IsActive()) {
    did_associate_form_controls_timer_.StartOneShot(
        TimeDelta::FromMilliseconds(300), FROM_HERE);
  }
}
