void SetUpV8() {
  gin::IsolateHolder::Initialize(gin::IsolateHolder::kNonStrictMode,
                                 gin::IsolateHolder::kStableV8Extras,
                                 gin::ArrayBufferAllocator::SharedInstance());
  DCHECK(!g_isolate_holder);
  g_isolate_holder = new gin::IsolateHolder(base::ThreadTaskRunnerHandle::Get(),
                                            gin::IsolateHolder::kSingleThread);
  g_isolate_holder->isolate()->Enter();
}
