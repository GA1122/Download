void FrameLoader::submitForm(PassRefPtr<FormSubmission> submission)
{
    ASSERT(submission->method() == FormSubmission::PostMethod || submission->method() == FormSubmission::GetMethod);

    ASSERT(submission->data());
    ASSERT(submission->state());
    ASSERT(submission->state()->sourceFrame() == m_frame);
    
    if (!m_frame->page())
        return;
    
    if (submission->action().isEmpty())
        return;

    if (isDocumentSandboxed(m_frame, SandboxForms))
        return;

    if (protocolIsJavaScript(submission->action())) {
        m_isExecutingJavaScriptFormAction = true;
        m_frame->script()->executeIfJavaScriptURL(submission->action(), false, DoNotReplaceDocumentIfJavaScriptURL);
        m_isExecutingJavaScriptFormAction = false;
        return;
    }

    Frame* targetFrame = m_frame->tree()->find(submission->target());
    if (!shouldAllowNavigation(targetFrame))
        return;
    if (!targetFrame) {
        if (!DOMWindow::allowPopUp(m_frame) && !isProcessingUserGesture())
            return;

        targetFrame = m_frame;
    } else
        submission->clearTarget();

    if (!targetFrame->page())
        return;




    if (m_frame->tree()->isDescendantOf(targetFrame)) {
        if (m_submittedFormURL == submission->action())
            return;
        m_submittedFormURL = submission->action();
    }

    submission->data()->generateFiles(m_frame->document());
    submission->setReferrer(m_outgoingReferrer);
    submission->setOrigin(outgoingOrigin());

    targetFrame->redirectScheduler()->scheduleFormSubmission(submission);
}
