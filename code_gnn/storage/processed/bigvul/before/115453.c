static WTF::String securityOriginToStr(WKSecurityOriginRef origin)
{
    StringBuilder stringBuilder;
    stringBuilder.append('{');
    stringBuilder.append(toWTFString(adoptWK(WKSecurityOriginCopyProtocol(origin))));
    stringBuilder.appendLiteral(", ");
    stringBuilder.append(toWTFString(adoptWK(WKSecurityOriginCopyHost(origin))));
    stringBuilder.appendLiteral(", ");
    stringBuilder.appendNumber(WKSecurityOriginGetPort(origin));
    stringBuilder.append('}');

    return stringBuilder.toString();
}
