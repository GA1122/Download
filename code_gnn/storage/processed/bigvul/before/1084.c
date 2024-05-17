cmsHPROFILE GfxColorSpace::loadColorProfile(const char *fileName)
{
  cmsHPROFILE hp = NULL;
  FILE *fp;

  if (fileName[0] == '/') {
    if ((fp = fopen(fileName,"r")) != NULL) {
      fclose(fp);
      hp = cmsOpenProfileFromFile(fileName,"r");
    }
    return hp;
  }
  GooString *path = globalParams->getBaseDir();
  path->append(COLOR_PROFILE_DIR);
  path->append(fileName);
  if ((fp = fopen(path->getCString(),"r")) != NULL) {
    fclose(fp);
    hp = cmsOpenProfileFromFile(path->getCString(),"r");
  }
  delete path;
  if (hp == NULL) {
    path = new GooString(GLOBAL_COLOR_PROFILE_DIR);
    path->append(fileName);
    if ((fp = fopen(path->getCString(),"r")) != NULL) {
      fclose(fp);
      hp = cmsOpenProfileFromFile(path->getCString(),"r");
    }
    delete path;
  }
  return hp;
}
