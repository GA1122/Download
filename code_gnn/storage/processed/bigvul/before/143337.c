WebSecurityOrigin WebFrame::getSecurityOrigin() const
{
    return WebSecurityOrigin(toImplBase()->frame()->securityContext()->getSecurityOrigin());
}
