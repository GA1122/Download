void Document::CountDetachingNodeAccessInDOMNodeRemovedHandler() {
  auto state = GetInDOMNodeRemovedHandlerState();
  DCHECK_NE(state, InDOMNodeRemovedHandlerState::kNone);
  UseCounter::Count(
      *this,
      state == InDOMNodeRemovedHandlerState::kDOMNodeRemoved
          ? WebFeature::kDOMNodeRemovedEventHandlerAccessDetachingNode
          : WebFeature::
                kDOMNodeRemovedFromDocumentEventHandlerAccessDetachingNode);
}
