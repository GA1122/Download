bool CParaNdisTX::SendMapped(bool IsInterrupt, PNET_BUFFER_LIST &NBLFailNow)
{
    if(!ParaNdis_IsSendPossible(m_Context))
    {
        NBLFailNow = RemoveAllNonWaitingNBLs();
        if (NBLFailNow)
        {
            DPrintf(0, (__FUNCTION__ " Failing send"));
        }
    }
    else
    {
        bool SentOutSomeBuffers = false;
        auto HaveBuffers = true;

        while (HaveBuffers && HaveMappedNBLs())
        {
            auto NBLHolder = PopMappedNBL();

            if (NBLHolder->HaveMappedBuffers())
            {
                auto NBHolder = NBLHolder->PopMappedNB();
                auto result = m_VirtQueue.SubmitPacket(*NBHolder);

                switch (result)
                {
                case SUBMIT_NO_PLACE_IN_QUEUE:
                    NBLHolder->PushMappedNB(NBHolder);
                    PushMappedNBL(NBLHolder);
                    HaveBuffers = false;
                    break;

                case SUBMIT_FAILURE:
                case SUBMIT_SUCCESS:
                case SUBMIT_PACKET_TOO_LARGE:
                    if (!NBLHolder->HaveMappedBuffers())
                    {
                        m_WaitingList.Push(NBLHolder);
                    }
                    else
                    {
                        PushMappedNBL(NBLHolder);
                    }

                    if (result == SUBMIT_SUCCESS)
                    {
                        SentOutSomeBuffers = true;
                    }
                    else
                    {
                        NBHolder->SendComplete();
                        CNB::Destroy(NBHolder, m_Context->MiniportHandle);
                    }
                    break;
                default:
                    ASSERT(false);
                    break;
                }
            }
            else
            {

                m_WaitingList.Push(NBLHolder);
            }
        }

        if (SentOutSomeBuffers)
        {
            DPrintf(2, ("[%s] sent down\n", __FUNCTION__, SentOutSomeBuffers));
            if (IsInterrupt)
            {
                return true;
            }
            else
            {
                m_VirtQueue.Kick();
            }
        }
    }

    return false;
}