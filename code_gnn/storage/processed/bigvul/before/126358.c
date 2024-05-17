int BrowserWindowGtk::GetThemeFrameResource() {
  bool incognito = browser()->profile()->IsOffTheRecord();
  int image_name;
  if (DrawFrameAsActive()) {
    image_name = incognito ? IDR_THEME_FRAME_INCOGNITO : IDR_THEME_FRAME;
  } else {
    image_name = incognito ? IDR_THEME_FRAME_INCOGNITO_INACTIVE :
                 IDR_THEME_FRAME_INACTIVE;
  }

  return image_name;
}
