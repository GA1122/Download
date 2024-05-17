bool Document::isSecureContext(const SecureContextCheck privilegeContextCheck) const
{
    return isSecureContextImpl(privilegeContextCheck);
}
