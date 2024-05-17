WebInsecureRequestPolicy WebFrame::getInsecureRequestPolicy() const
{
    return toImplBase()->frame()->securityContext()->getInsecureRequestPolicy();
}
