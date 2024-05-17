DEFINE_TRACE_WRAPPERS(Document) {
  visitor->TraceWrappers(imports_controller_);
  visitor->TraceWrappers(parser_);
  visitor->TraceWrappers(implementation_);
  visitor->TraceWrappers(style_sheet_list_);
  visitor->TraceWrappers(style_engine_);
  visitor->TraceWrappersWithManualWriteBarrier(
      static_cast<FontFaceSet*>(Supplementable<Document>::supplements_.at(
          FontFaceSet::SupplementName())));
  ContainerNode::TraceWrappers(visitor);
}
