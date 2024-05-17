void Document::TraceWrappers(const ScriptWrappableVisitor* visitor) const {
  visitor->TraceWrappers(imports_controller_);
  visitor->TraceWrappers(parser_);
  visitor->TraceWrappers(implementation_);
  visitor->TraceWrappers(style_sheet_list_);
  visitor->TraceWrappers(style_engine_);
  visitor->TraceWrappers(script_runner_);
  visitor->TraceWrappers(scripted_animation_controller_);
  visitor->TraceWrappers(scripted_idle_task_controller_);
  ContainerNode::TraceWrappers(visitor);
  Supplementable<Document>::TraceWrappers(visitor);
}
