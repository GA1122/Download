String Document::queryCommandValue(const String& commandName)
{
    return command(this, commandName).value();
}
