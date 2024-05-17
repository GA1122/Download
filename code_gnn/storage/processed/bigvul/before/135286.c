bool Document::isSecureContext(String& errorMessage, const SecureContextCheck privilegeContextCheck) const
{
    return isSecureContextImpl(&errorMessage, privilegeContextCheck);
}
