void Document::requestFullScreenForElement(Element* element, unsigned short flags, FullScreenCheckType checkType)
{
    bool inLegacyMozillaMode = (flags & Element::LEGACY_MOZILLA_REQUEST);

    do {
        if (!element)
            element = documentElement();
 

        if (!element->inDocument())
            break;

        if (checkType == EnforceIFrameAllowFullScreenRequirement && !fullScreenIsAllowedForElement(element))
            break;

        if (!m_fullScreenElementStack.isEmpty() && !m_fullScreenElementStack.last()->contains(element) && !inLegacyMozillaMode)
            break;

        bool descendentHasNonEmptyStack = false;
        for (Frame* descendant = frame() ? frame()->tree()->traverseNext() : 0; descendant; descendant = descendant->tree()->traverseNext()) {
            if (descendant->document()->webkitFullscreenElement()) {
                descendentHasNonEmptyStack = true;
                break;
            }
        }
        if (descendentHasNonEmptyStack && !inLegacyMozillaMode)
            break;

        if (!ScriptController::processingUserGesture())
            break;

        if (!page() || !page()->settings()->fullScreenEnabled())
            break;

        if (!page()->chrome()->client()->supportsFullScreenForElement(element, flags & Element::ALLOW_KEYBOARD_INPUT)) {
            if (!inLegacyMozillaMode && flags & Element::ALLOW_KEYBOARD_INPUT) {
                flags &= ~Element::ALLOW_KEYBOARD_INPUT;
                if (!page()->chrome()->client()->supportsFullScreenForElement(element, false))
                    break;
            } else
                break;
        }

        Document* currentDoc = this;

        Deque<Document*> docs;

        do {
            docs.prepend(currentDoc);
            currentDoc = currentDoc->ownerElement() ? currentDoc->ownerElement()->document() : 0;
        } while (currentDoc);

        Deque<Document*>::iterator current = docs.begin(), following = docs.begin();

        do {
            ++following;

            Document* currentDoc = *current;
            Document* followingDoc = following != docs.end() ? *following : 0;

            if (!followingDoc) {
                currentDoc->pushFullscreenElementStack(element);
                addDocumentToFullScreenChangeEventQueue(currentDoc);
                continue;
            }

            Element* topElement = currentDoc->webkitFullscreenElement();
            if (!topElement || topElement != followingDoc->ownerElement()) {
                currentDoc->pushFullscreenElementStack(followingDoc->ownerElement());
                addDocumentToFullScreenChangeEventQueue(currentDoc);
                continue;
            }

        } while (++current != docs.end());

        m_areKeysEnabledInFullScreen = flags & Element::ALLOW_KEYBOARD_INPUT;
        page()->chrome()->client()->enterFullScreenForElement(element);

        return;
    } while (0);

    m_fullScreenErrorEventTargetQueue.append(element ? element : documentElement());
    m_fullScreenChangeDelayTimer.startOneShot(0);
}
