void RenderView::didCreateIsolatedScriptContext(WebFrame* frame) {
  content::GetContentClient()->renderer()->DidCreateIsolatedScriptContext(
      frame);
}
