bool InputHandler::setCursorPosition(int location)
{
    return setSelection(location, location);
}
