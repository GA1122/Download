void RenderThread::RegisterExtension(v8::Extension* extension) {
  WebScriptController::registerExtension(extension);
  v8_extensions_.insert(extension->name());
}
