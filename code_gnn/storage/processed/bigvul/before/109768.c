bool Document::queryCommandEnabled(const String& commandName)
{
    return command(this, commandName).isEnabled();
}
