bool ResourceFetcher::checkInsecureContent(Resource::Type type, const KURL& url, MixedContentBlockingTreatment treatment) const
{
    if (treatment == TreatAsDefaultForType) {
        switch (type) {
        case Resource::XSLStyleSheet:
            ASSERT(RuntimeEnabledFeatures::xsltEnabled());
        case Resource::Script:
        case Resource::SVGDocument:
        case Resource::CSSStyleSheet:
        case Resource::ImportResource:
            treatment = TreatAsActiveContent;
            break;

        case Resource::Font:
            treatment = TreatAsActiveContent;
            break;

        case Resource::TextTrack:
        case Resource::Raw:
        case Resource::Image:
        case Resource::Media:
            treatment = TreatAsPassiveContent;
            break;

        case Resource::MainResource:
        case Resource::LinkPrefetch:
        case Resource::LinkSubresource:
            treatment = TreatAsAlwaysAllowedContent;
            break;
        }
    }
    if (frame() && !frame()->tree().top()->isLocalFrame())
        return false;
    if (treatment == TreatAsActiveContent) {
        if (LocalFrame* f = frame()) {
            if (!f->loader().mixedContentChecker()->canRunInsecureContent(m_document->securityOrigin(), url))
                return false;
            Frame* top = f->tree().top();
            if (top != f && !toLocalFrame(top)->loader().mixedContentChecker()->canRunInsecureContent(toLocalFrame(top)->document()->securityOrigin(), url))
                return false;
        }
    } else if (treatment == TreatAsPassiveContent) {
        if (LocalFrame* f = frame()) {
            Frame* top = f->tree().top();
            if (!toLocalFrame(top)->loader().mixedContentChecker()->canDisplayInsecureContent(toLocalFrame(top)->document()->securityOrigin(), url))
                return false;
            if (MixedContentChecker::isMixedContent(toLocalFrame(top)->document()->securityOrigin(), url)) {
                switch (type) {
                case Resource::TextTrack:
                    UseCounter::count(toLocalFrame(top)->document(), UseCounter::MixedContentTextTrack);
                    break;

                case Resource::Raw:
                    UseCounter::count(toLocalFrame(top)->document(), UseCounter::MixedContentRaw);
                    break;

                case Resource::Image:
                    UseCounter::count(toLocalFrame(top)->document(), UseCounter::MixedContentImage);
                    break;

                case Resource::Media:
                    UseCounter::count(toLocalFrame(top)->document(), UseCounter::MixedContentMedia);
                    break;

                default:
                    ASSERT_NOT_REACHED();
                }
            }
        }
    } else {
        ASSERT(treatment == TreatAsAlwaysAllowedContent);
    }
    return true;
}
