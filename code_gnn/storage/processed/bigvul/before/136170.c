String Document::outgoingReferrer()
{
    if (securityOrigin()->isUnique()) {
        return String();
    }

    Document* referrerDocument = this;
    if (LocalFrame* frame = m_frame) {
        while (frame->document()->isSrcdocDocument()) {
            frame = toLocalFrame(frame->tree().parent());
            ASSERT(frame);
        }
        referrerDocument = frame->document();
    }
    return referrerDocument->m_url.strippedForUseAsReferrer();
}
