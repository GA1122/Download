static bool thumbUnderMouse(Scrollbar* scrollbar)
{
    int thumbPos = scrollbar->theme()->trackPosition(scrollbar) + scrollbar->theme()->thumbPosition(scrollbar);
    int thumbLength = scrollbar->theme()->thumbLength(scrollbar);
    return scrollbar->pressedPos() >= thumbPos && scrollbar->pressedPos() < thumbPos + thumbLength;
}
