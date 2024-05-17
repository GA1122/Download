void LocalDOMWindow::cancelAnimationFrame(int id) {
  if (Document* document = this->document())
    document->CancelAnimationFrame(id);
}
