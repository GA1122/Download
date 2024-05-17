void Document::SetAutofocusElement(Element* element) {
  if (!element) {
    autofocus_element_ = nullptr;
    return;
  }
  if (has_autofocused_)
    return;
  has_autofocused_ = true;
  DCHECK(!autofocus_element_);
  autofocus_element_ = element;
  GetTaskRunner(TaskType::kUserInteraction)
      ->PostTask(BLINK_FROM_HERE,
                 WTF::Bind(&RunAutofocusTask, WrapWeakPersistent(this)));
}
