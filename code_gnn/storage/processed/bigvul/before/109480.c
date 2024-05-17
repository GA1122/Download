unsigned ScriptProfiler::requestHeapStatsUpdate(ScriptProfiler::OutputStream* stream)
{
    HeapStatsStream heapStatsStream(stream);
    return v8::Isolate::GetCurrent()->GetHeapProfiler()->GetHeapStats(&heapStatsStream);
}
