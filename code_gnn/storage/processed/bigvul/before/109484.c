PassRefPtr<ScriptProfile> ScriptProfiler::stop(const String& title)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::CpuProfiler* profiler = isolate->GetCpuProfiler();
    if (!profiler)
        return 0;
    v8::HandleScope handleScope(isolate);
    const v8::CpuProfile* profile = profiler->StopCpuProfiling(v8String(title, isolate));
    if (!profile)
        return 0;

    String profileTitle = toWebCoreString(profile->GetTitle());
    double idleTime = 0.0;
    ProfileNameIdleTimeMap* profileNameIdleTimeMap = ScriptProfiler::currentProfileNameIdleTimeMap();
    ProfileNameIdleTimeMap::iterator profileIdleTime = profileNameIdleTimeMap->find(profileTitle);
    if (profileIdleTime != profileNameIdleTimeMap->end()) {
        idleTime = profileIdleTime->value * 1000.0;
        profileNameIdleTimeMap->remove(profileIdleTime);
    }

    return ScriptProfile::create(profile, idleTime);
}
