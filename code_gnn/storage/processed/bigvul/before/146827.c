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
  TaskRunnerHelper::Get(TaskType::kUserInteraction, this)
      ->PostTask(BLINK_FROM_HERE,
                 WTF::Bind(&RunAutofocusTask, WrapWeakPersistent(this)));
}
