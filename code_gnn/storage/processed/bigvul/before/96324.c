bool CNBL::ParsePriority()
{
    NDIS_NET_BUFFER_LIST_8021Q_INFO priorityInfo;
    priorityInfo.Value = m_Context->ulPriorityVlanSetting ?
        NET_BUFFER_LIST_INFO(m_NBL, Ieee8021QNetBufferListInfo) : nullptr;

    if (!priorityInfo.TagHeader.VlanId)
    {
        priorityInfo.TagHeader.VlanId = m_Context->VlanId;
    }

    if (priorityInfo.TagHeader.CanonicalFormatId || !IsValidVlanId(m_Context, priorityInfo.TagHeader.VlanId))
    {
        DPrintf(0, ("[%s] Discarded invalid priority tag %p\n", __FUNCTION__, priorityInfo.Value));
        return false;
    }
    else if (priorityInfo.Value)
    {
        if (!IsPrioritySupported(m_Context))
            priorityInfo.TagHeader.UserPriority = 0;
        if (!IsVlanSupported(m_Context))
            priorityInfo.TagHeader.VlanId = 0;
        if (priorityInfo.Value)
        {
            m_TCI = static_cast<UINT16>(priorityInfo.TagHeader.UserPriority << 13 | priorityInfo.TagHeader.VlanId);
            DPrintf(1, ("[%s] Populated priority tag %p\n", __FUNCTION__, priorityInfo.Value));
        }
    }

    return true;
}