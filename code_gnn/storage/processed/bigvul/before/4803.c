ConfineToShape(DeviceIntPtr pDev, RegionPtr shape, int *px, int *py)
{
    BoxRec box;
    int x = *px, y = *py;
    int incx = 1, incy = 1;

    if (RegionContainsPoint(shape, x, y, &box))
        return;
    box = *RegionExtents(shape);
     
    do {
        x += incx;
        if (x >= box.x2) {
            incx = -1;
            x = *px - 1;
        }
        else if (x < box.x1) {
            incx = 1;
            x = *px;
            y += incy;
            if (y >= box.y2) {
                incy = -1;
                y = *py - 1;
            }
            else if (y < box.y1)
                return;          
        }
    } while (!RegionContainsPoint(shape, x, y, &box));
    *px = x;
    *py = y;
}
