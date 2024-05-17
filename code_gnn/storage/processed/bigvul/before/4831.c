FindChildForEvent(SpritePtr pSprite, WindowPtr event)
{
    WindowPtr w = DeepestSpriteWin(pSprite);
    Window child = None;

     
    while (w) {
         

        if (w == event) {
            child = None;
            break;
        }

        if (w->parent == event) {
            child = w->drawable.id;
            break;
        }
        w = w->parent;
    }
    return child;
}
