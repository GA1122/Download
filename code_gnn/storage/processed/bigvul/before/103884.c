void RenderView::didDestroyScriptContext(WebFrame* frame) {
  content::GetContentClient()->renderer()->DidDestroyScriptContext(frame);
}
