void HTMLImportsController::Trace(blink::Visitor* visitor) {
  visitor->Trace(root_);
  visitor->Trace(loaders_);
}
