static void LiveNodeListBaseWriteBarrier(void* parent,
                                         const LiveNodeListBase* list) {
  if (IsHTMLCollectionType(list->GetType())) {
    ScriptWrappableMarkingVisitor::WriteBarrier(
        static_cast<const HTMLCollection*>(list));
  } else {
    ScriptWrappableMarkingVisitor::WriteBarrier(
        static_cast<const LiveNodeList*>(list));
  }
}
