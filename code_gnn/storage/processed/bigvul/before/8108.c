void Gfx::opBeginMarkedContent(Object args[], int numArgs) {
  pushMarkedContent();
  
  OCGs *contentConfig = catalog->getOptContentConfig();
  char* name0 = args[0].getName();
  if ( strncmp( name0, "OC", 2) == 0 && contentConfig) {
    if ( numArgs >= 2 ) {
      if (!args[1].isName()) {
	error(getPos(), "Unexpected MC Type: %i", args[1].getType());
      }
      char* name1 = args[1].getName();
      Object markedContent;
      if ( res->lookupMarkedContentNF( name1, &markedContent ) ) {
	if ( markedContent.isRef() ) {
	  bool visible = contentConfig->optContentIsVisible( &markedContent );
	  MarkedContentStack *mc = mcStack;
	  mc->ocSuppressed = !(visible);
       }
      } else {
	error(getPos(), "DID NOT find %s", name1);
      }
    } else {
      error(getPos(), "insufficient arguments for Marked Content");
    }
  }

  if (printCommands) {
    printf("  marked content: %s ", args[0].getName());
    if (numArgs == 2)
      args[1].print(stdout);
    printf("\n");
    fflush(stdout);
  }

  if(numArgs == 2 && args[1].isDict ()) {
    out->beginMarkedContent(args[0].getName(),args[1].getDict());
  } else if(numArgs == 1) {
    out->beginMarkedContent(args[0].getName(),NULL);
  }
}
