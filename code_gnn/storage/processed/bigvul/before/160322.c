  Task(ImageLoader* loader,
       UpdateFromElementBehavior update_behavior,
       ReferrerPolicy referrer_policy)
      : loader_(loader),
        should_bypass_main_world_csp_(ShouldBypassMainWorldCSP(loader)),
        update_behavior_(update_behavior),
        weak_factory_(this),
        referrer_policy_(referrer_policy) {
    ExecutionContext& context = loader_->GetElement()->GetDocument();
    probe::AsyncTaskScheduled(&context, "Image", this);
    v8::Isolate* isolate = V8PerIsolateData::MainThreadIsolate();
    v8::HandleScope scope(isolate);
    if (!isolate->GetCurrentContext().IsEmpty()) {
      script_state_ = ScriptState::Current(isolate);
    } else {
      script_state_ = ToScriptStateForMainWorld(
          loader->GetElement()->GetDocument().GetFrame());
      DCHECK(script_state_);
    }
    request_url_ =
        loader->ImageSourceToKURL(loader->GetElement()->ImageSourceURL());
  }
