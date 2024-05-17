 void HTMLStyleElement::ChildrenChanged(const ChildrenChange& change) {
   HTMLElement::ChildrenChanged(change);
   if (StyleElement::ChildrenChanged(*this) ==
      StyleElement::kProcessingFatalError)
    NotifyLoadedSheetAndAllCriticalSubresources(
        kErrorOccurredLoadingSubresource);
}
