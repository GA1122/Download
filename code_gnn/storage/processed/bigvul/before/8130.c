void Gfx::opMarkPoint(Object args[], int numArgs) {
  if (printCommands) {
    printf("  mark point: %s ", args[0].getName());
    if (numArgs == 2)
      args[1].print(stdout);
    printf("\n");
    fflush(stdout);
  }

  if(numArgs == 2 && args[1].isDict()) {
    out->markPoint(args[0].getName(),args[1].getDict());
  } else {
    out->markPoint(args[0].getName());
  }

}
