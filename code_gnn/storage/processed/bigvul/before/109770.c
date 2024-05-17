bool Document::queryCommandState(const String& commandName)
{
    return command(this, commandName).state() == TrueTriState;
}
