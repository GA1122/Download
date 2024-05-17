static void LiveNodeListBaseWriteBarrier(void* parent,
                                         const LiveNodeListBase* list) {
  if (IsHTMLCollectionType(list->GetType())) {
    ScriptWrappableVisitor::WriteBarrier(
        static_cast<const HTMLCollection*>(list));
  } else {
    ScriptWrappableVisitor::WriteBarrier(
        static_cast<const LiveNodeList*>(list));
  }
}
