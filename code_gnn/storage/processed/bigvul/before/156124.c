void HTMLLinkElement::Process() {
  if (LinkResource* link = LinkResourceToProcess())
    link->Process();
}
