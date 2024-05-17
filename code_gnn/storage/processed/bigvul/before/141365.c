void Document::setRootScroller(Element* new_scroller, ExceptionState&) {
  root_scroller_controller_->Set(new_scroller);
}
