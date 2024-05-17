void DisableDetectFlowFileFlags(Flow *f)
{
    DetectPostInspectFileFlagsUpdate(f, NULL  , STREAM_TOSERVER);
    DetectPostInspectFileFlagsUpdate(f, NULL  , STREAM_TOCLIENT);
}
