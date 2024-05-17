void HTMLConstructionSite::fosterParent(Node* node)
{
    AttachmentSite site;
    findFosterSite(site);
    attachAtSite(site, node);
}
