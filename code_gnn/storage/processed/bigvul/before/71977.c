static XMLTreeInfo *ParseCloseTag(XMLTreeRoot *root,char *tag,
  ExceptionInfo *exception)
{
  if ((root->node == (XMLTreeInfo *) NULL) ||
      (root->node->tag == (char *) NULL) || (strcmp(tag,root->node->tag) != 0))
    {
      (void) ThrowMagickException(exception,GetMagickModule(),OptionWarning,
        "ParseError","unexpected closing tag </%s>",tag);
      return(&root->root);
    }
  root->node=root->node->parent;
  return((XMLTreeInfo *) NULL);
}
