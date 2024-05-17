PassRefPtr<Text> Document::createTextNode(const String& data)
{
    return Text::create(*this, data);
}
