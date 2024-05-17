void Document::PopCurrentScript(ScriptElementBase* script) {
  DCHECK(!current_script_stack_.IsEmpty());
  DCHECK_EQ(current_script_stack_.back(), script);
  current_script_stack_.pop_back();
}
