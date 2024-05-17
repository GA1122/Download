void ScriptProfiler::start(const String& title)
{
    ProfileNameIdleTimeMap* profileNameIdleTimeMap = ScriptProfiler::currentProfileNameIdleTimeMap();
    if (profileNameIdleTimeMap->contains(title))
        return;
    profileNameIdleTimeMap->add(title, 0);

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::CpuProfiler* profiler = isolate->GetCpuProfiler();
    if (!profiler)
        return;
    v8::HandleScope handleScope(isolate);
    profiler->StartCpuProfiling(v8String(title, isolate), true);
}
