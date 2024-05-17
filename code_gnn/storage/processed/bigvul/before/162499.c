ClassicPendingScript* ClassicPendingScript::CreateInline(
    ScriptElementBase* element,
    const TextPosition& starting_position,
    ScriptSourceLocationType source_location_type,
    const ScriptFetchOptions& options) {
  ClassicPendingScript* pending_script =
      new ClassicPendingScript(element, starting_position, source_location_type,
                               options, false  );
  pending_script->CheckState();
  return pending_script;
}
