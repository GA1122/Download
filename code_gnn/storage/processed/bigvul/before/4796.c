CheckMotion(DeviceEvent *ev, DeviceIntPtr pDev)
{
    WindowPtr prevSpriteWin, newSpriteWin;
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    verify_internal_event((InternalEvent *) ev);

    prevSpriteWin = pSprite->win;

    if (ev && !syncEvents.playingEvents) {
         
        switch (ev->type) {
        case ET_ButtonPress:
        case ET_ButtonRelease:
        case ET_Motion:
        case ET_TouchBegin:
        case ET_TouchUpdate:
        case ET_TouchEnd:
            break;
        default:
             
            return FALSE;
        }

#ifdef PANORAMIX
        if (!noPanoramiXExtension) {
             
            ev->root_x += pSprite->screen->x - screenInfo.screens[0]->x;
            ev->root_y += pSprite->screen->y - screenInfo.screens[0]->y;
        }
        else
#endif
        {
            if (pSprite->hot.pScreen != pSprite->hotPhys.pScreen) {
                pSprite->hot.pScreen = pSprite->hotPhys.pScreen;
                RootWindow(pDev->spriteInfo->sprite) =
                    pSprite->hot.pScreen->root;
            }
        }

        pSprite->hot.x = ev->root_x;
        pSprite->hot.y = ev->root_y;
        if (pSprite->hot.x < pSprite->physLimits.x1)
            pSprite->hot.x = pSprite->physLimits.x1;
        else if (pSprite->hot.x >= pSprite->physLimits.x2)
            pSprite->hot.x = pSprite->physLimits.x2 - 1;
        if (pSprite->hot.y < pSprite->physLimits.y1)
            pSprite->hot.y = pSprite->physLimits.y1;
        else if (pSprite->hot.y >= pSprite->physLimits.y2)
            pSprite->hot.y = pSprite->physLimits.y2 - 1;
        if (pSprite->hotShape)
            ConfineToShape(pDev, pSprite->hotShape, &pSprite->hot.x,
                           &pSprite->hot.y);
        pSprite->hotPhys = pSprite->hot;

        if ((pSprite->hotPhys.x != ev->root_x) ||
            (pSprite->hotPhys.y != ev->root_y)) {
#ifdef PANORAMIX
            if (!noPanoramiXExtension) {
                XineramaSetCursorPosition(pDev, pSprite->hotPhys.x,
                                          pSprite->hotPhys.y, FALSE);
            }
            else
#endif
            {
                (*pSprite->hotPhys.pScreen->SetCursorPosition) (pDev,
                                                                pSprite->
                                                                hotPhys.pScreen,
                                                                pSprite->
                                                                hotPhys.x,
                                                                pSprite->
                                                                hotPhys.y,
                                                                FALSE);
            }
        }

        ev->root_x = pSprite->hot.x;
        ev->root_y = pSprite->hot.y;
    }

    newSpriteWin = XYToWindow(pSprite, pSprite->hot.x, pSprite->hot.y);

    if (newSpriteWin != prevSpriteWin) {
        int sourceid;

        if (!ev) {
            UpdateCurrentTimeIf();
            sourceid = pDev->id;         
        }
        else
            sourceid = ev->sourceid;

        if (prevSpriteWin != NullWindow) {
            if (!ActivateEnterGrab(pDev, prevSpriteWin, newSpriteWin))
                DoEnterLeaveEvents(pDev, sourceid, prevSpriteWin,
                                   newSpriteWin, NotifyNormal);
        }
         
        pSprite->win = newSpriteWin;
        PostNewCursor(pDev);
        return FALSE;
    }
    return TRUE;
}
