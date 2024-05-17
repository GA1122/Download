void RenderView::didCreateScriptContext(WebFrame* frame) {
  content::GetContentClient()->renderer()->DidCreateScriptContext(frame);
}
