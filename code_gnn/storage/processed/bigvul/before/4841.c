GetSpritePosition(DeviceIntPtr pDev, int *px, int *py)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    *px = pSprite->hotPhys.x;
    *py = pSprite->hotPhys.y;
}
