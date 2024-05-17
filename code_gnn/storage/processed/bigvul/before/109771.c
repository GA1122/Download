bool Document::queryCommandSupported(const String& commandName)
{
    return command(this, commandName).isSupported();
}
