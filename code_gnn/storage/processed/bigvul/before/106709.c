static void flashRects(HDC dc, const IntRect rects[], size_t rectCount, HBRUSH brush)
{
    for (size_t i = 0; i < rectCount; ++i) {
        RECT winRect = rects[i];
        ::FillRect(dc, &winRect, brush);
    }

    ::GdiFlush();
    ::Sleep(50);
}
