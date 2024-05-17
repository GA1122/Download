void CairoOutputDev::beginMarkedContent(char *name, Dict *properties)
{
  if (text)
    actualText->beginMC(properties);
}
