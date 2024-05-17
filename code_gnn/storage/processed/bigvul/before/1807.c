main (int argc, char *argv[])
{
  Object info;
  GBool ok;
  int exitCode;

  exitCode = 99;

  ok = parseArgs (argDesc, &argc, argv);
  if (!ok || argc != 3 || printVersion || printHelp)
    {
      fprintf (stderr, "pdfseparate version %s\n", PACKAGE_VERSION);
      fprintf (stderr, "%s\n", popplerCopyright);
      fprintf (stderr, "%s\n", xpdfCopyright);
      if (!printVersion)
	{
	  printUsage ("pdfseparate", "<PDF-sourcefile> <PDF-pattern-destfile>",
		      argDesc);
	}
      if (printVersion || printHelp)
	exitCode = 0;
      goto err0;
    }
  globalParams = new GlobalParams();
  ok = extractPages (argv[1], argv[2]);
  if (ok) {
    exitCode = 0;
  }
  delete globalParams;

err0:

  return exitCode;
}
