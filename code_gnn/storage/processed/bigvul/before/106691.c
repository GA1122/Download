static OwnPtr<HBRUSH> createBrush(const Color& color)
{
    return adoptPtr(::CreateSolidBrush(RGB(color.red(), color.green(), color.blue())));
}
