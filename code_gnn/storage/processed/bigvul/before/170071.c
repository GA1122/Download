xsltFreeTemplate(xsltTemplatePtr template) {
    if (template == NULL)
	return;
    if (template->match) xmlFree(template->match);
 
 
    if (template->inheritedNs) xmlFree(template->inheritedNs);

     
    if (template->templCalledTab) xmlFree(template->templCalledTab);
    if (template->templCountTab) xmlFree(template->templCountTab);

    memset(template, -1, sizeof(xsltTemplate));
    xmlFree(template);
}
