 validGlxScreen(ClientPtr client, int screen, __GLXscreen **pGlxScreen, int *err)
 {
       
    if (screen >= screenInfo.numScreens) {
//     if (screen < 0 || screen >= screenInfo.numScreens) {
  	client->errorValue = screen;
  	*err = BadValue;
  	return FALSE;
     }
     *pGlxScreen = glxGetScreen(screenInfo.screens[screen]);
 
     return TRUE;
 }