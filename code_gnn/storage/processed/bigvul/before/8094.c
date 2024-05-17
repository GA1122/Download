void Gfx::go(GBool topLevel) {
  Object obj;
  Object args[maxArgs];
  int numArgs, i;
  int lastAbortCheck;

  pushStateGuard();
  updateLevel = lastAbortCheck = 0;
  numArgs = 0;
  parser->getObj(&obj);
  while (!obj.isEOF()) {
    commandAborted = gFalse;

    if (obj.isCmd()) {
      if (printCommands) {
	obj.print(stdout);
	for (i = 0; i < numArgs; ++i) {
	  printf(" ");
	  args[i].print(stdout);
	}
	printf("\n");
	fflush(stdout);
      }
      GooTimer timer;

      execOp(&obj, args, numArgs);

      if (profileCommands) {
	GooHash *hash;

	hash = out->getProfileHash ();
	if (hash) {
	  GooString *cmd_g;
	  ProfileData *data_p;

	  cmd_g = new GooString (obj.getCmd());
	  data_p = (ProfileData *)hash->lookup (cmd_g);
	  if (data_p == NULL) {
	    data_p = new ProfileData();
	    hash->add (cmd_g, data_p);
	  }
	  
	  data_p->addElement(timer.getElapsed ());
	}
      }
      obj.free();
      for (i = 0; i < numArgs; ++i)
	args[i].free();
      numArgs = 0;

      if (++updateLevel >= 20000) {
	out->dump();
	updateLevel = 0;
      }

      if (commandAborted) {
	commandAborted = gFalse;
	break;
      }

      if (abortCheckCbk) {
	if (updateLevel - lastAbortCheck > 10) {
	  if ((*abortCheckCbk)(abortCheckCbkData)) {
	    break;
	  }
	  lastAbortCheck = updateLevel;
	}
      }

    } else if (numArgs < maxArgs) {
      args[numArgs++] = obj;

    } else {
      error(getPos(), "Too many args in content stream");
      if (printCommands) {
	printf("throwing away arg: ");
	obj.print(stdout);
	printf("\n");
	fflush(stdout);
      }
      obj.free();
    }

    parser->getObj(&obj);
  }
  obj.free();

  if (numArgs > 0) {
    error(getPos(), "Leftover args in content stream");
    if (printCommands) {
      printf("%d leftovers:", numArgs);
      for (i = 0; i < numArgs; ++i) {
	printf(" ");
	args[i].print(stdout);
      }
      printf("\n");
      fflush(stdout);
    }
    for (i = 0; i < numArgs; ++i)
      args[i].free();
  }

  popStateGuard();

  if (topLevel && updateLevel > 0) {
    out->dump();
  }
}
