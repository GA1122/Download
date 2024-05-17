void BlinkTestRunner::EvaluateInWebInspector(long call_id,
                                             const std::string& script) {
  WebDevToolsAgent* agent =
      render_view()->GetMainRenderFrame()->GetWebFrame()->devToolsAgent();
  if (agent)
    agent->evaluateInWebInspector(call_id, WebString::fromUTF8(script));
}
