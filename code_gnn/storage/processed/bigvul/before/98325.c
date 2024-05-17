void FrameLoaderClient::dispatchWillSubmitForm(FramePolicyFunction policyFunction, PassRefPtr<FormState>)
{
    ASSERT(policyFunction);
    if (!policyFunction)
        return;
    (core(m_frame)->loader()->policyChecker()->*policyFunction)(PolicyUse);
}
