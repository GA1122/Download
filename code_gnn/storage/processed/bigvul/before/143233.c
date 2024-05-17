String Document::outgoingReferrer() const
{
    if (getSecurityOrigin()->isUnique()) {
        return String();
    }

    const Document* referrerDocument = this;
    if (LocalFrame* frame = m_frame) {
        while (frame->document()->isSrcdocDocument()) {
            frame = toLocalFrame(frame->tree().parent());
            DCHECK(frame);
        }
        referrerDocument = frame->document();
    }
    return referrerDocument->m_url.strippedForUseAsReferrer();
}
