PassRefPtr<DocumentFragment> createFragmentFromMarkupWithContext(Document* document, const String& markup, unsigned fragmentStart, unsigned fragmentEnd,
    const String& baseURL, FragmentScriptingPermission scriptingPermission)
{

    StringBuilder taggedMarkup;
    taggedMarkup.append(markup.left(fragmentStart));
    MarkupAccumulator::appendComment(taggedMarkup, fragmentMarkerTag);
    taggedMarkup.append(markup.substring(fragmentStart, fragmentEnd - fragmentStart));
    MarkupAccumulator::appendComment(taggedMarkup, fragmentMarkerTag);
    taggedMarkup.append(markup.substring(fragmentEnd));

    RefPtr<DocumentFragment> taggedFragment = createFragmentFromMarkup(document, taggedMarkup.toString(), baseURL, scriptingPermission);
    RefPtr<Document> taggedDocument = Document::create(0, KURL());
    taggedDocument->takeAllChildrenFrom(taggedFragment.get());

    RefPtr<Node> nodeBeforeContext;
    RefPtr<Node> nodeAfterContext;
    if (!findNodesSurroundingContext(taggedDocument.get(), nodeBeforeContext, nodeAfterContext))
        return 0;

    RefPtr<Range> range = Range::create(taggedDocument.get(),
        positionAfterNode(nodeBeforeContext.get()).parentAnchoredEquivalent(),
        positionBeforeNode(nodeAfterContext.get()).parentAnchoredEquivalent());

    ExceptionCode ec = 0;
    Node* commonAncestor = range->commonAncestorContainer(ec);
    ASSERT(!ec);
    Node* specialCommonAncestor = ancestorToRetainStructureAndAppearanceWithNoRenderer(commonAncestor);

    RefPtr<DocumentFragment> fragment = DocumentFragment::create(document);
    if (specialCommonAncestor) {
        fragment->appendChild(specialCommonAncestor, ec);
        ASSERT(!ec);
    } else
        fragment->takeAllChildrenFrom(static_cast<ContainerNode*>(commonAncestor));

    trimFragment(fragment.get(), nodeBeforeContext.get(), nodeAfterContext.get());

    return fragment;
}
