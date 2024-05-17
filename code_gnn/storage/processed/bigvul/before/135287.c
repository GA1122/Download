bool Document::isSecureContext(const SecureContextCheck privilegeContextCheck) const
{
    return isSecureContextImpl(nullptr, privilegeContextCheck);
}
