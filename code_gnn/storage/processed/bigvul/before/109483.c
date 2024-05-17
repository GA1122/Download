void ScriptProfiler::startTrackingHeapObjects()
{
    v8::Isolate::GetCurrent()->GetHeapProfiler()->StartTrackingHeapObjects();
}
