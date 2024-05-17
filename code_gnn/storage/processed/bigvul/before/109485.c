void ScriptProfiler::stopTrackingHeapObjects()
{
    v8::Isolate::GetCurrent()->GetHeapProfiler()->StopTrackingHeapObjects();
}
