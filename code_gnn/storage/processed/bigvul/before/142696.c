static bool needsHistoryItemRestore(FrameLoadType type)
{
    return type == FrameLoadTypeBackForward || type == FrameLoadTypeReload
        || type == FrameLoadTypeReloadBypassingCache;
}
