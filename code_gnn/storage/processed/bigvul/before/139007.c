  Task(ImageLoader* loader,
       UpdateFromElementBehavior updateBehavior,
       ReferrerPolicy referrerPolicy)
      : m_loader(loader),
        m_shouldBypassMainWorldCSP(shouldBypassMainWorldCSP(loader)),
        m_updateBehavior(updateBehavior),
        m_weakFactory(this),
        m_referrerPolicy(referrerPolicy) {
    ExecutionContext& context = m_loader->element()->document();
    InspectorInstrumentation::asyncTaskScheduled(&context, "Image", this);
    v8::Isolate* isolate = V8PerIsolateData::mainThreadIsolate();
    v8::HandleScope scope(isolate);
    if (!isolate->GetCurrentContext().IsEmpty()) {
      m_scriptState = ScriptState::current(isolate);
    } else {
      m_scriptState =
          ScriptState::forMainWorld(loader->element()->document().frame());
      DCHECK(m_scriptState);
    }
    m_requestURL =
        loader->imageSourceToKURL(loader->element()->imageSourceURL());
  }
