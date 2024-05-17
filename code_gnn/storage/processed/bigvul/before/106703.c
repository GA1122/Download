static void drawPageBackground(HDC dc, const RECT& rect)
{
    ::FillRect(dc, &rect, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
}
