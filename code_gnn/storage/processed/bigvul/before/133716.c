static bool isDangerousHTTPEquiv(const String& value)
{
    String equiv = value.stripWhiteSpace();
    return equalIgnoringCase(equiv, "refresh") || equalIgnoringCase(equiv, "set-cookie");
}
