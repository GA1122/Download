void HTMLConstructionSite::attachAtSite(const AttachmentSite& site, PassRefPtr<Node> prpChild)
{
    RefPtr<Node> child = prpChild;

    if (site.nextChild) {
        ExceptionCode ec = 0;
        site.parent->insertBefore(child, site.nextChild, ec);
        ASSERT(!ec);
        if (site.parent->attached() && !child->attached())
            child->attach();
        return;
    }
    site.parent->parserAddChild(child);
    if (site.parent->attached() && !child->attached())
        child->attach();
}
