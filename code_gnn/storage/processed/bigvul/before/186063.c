 bool Document::isSecureContextImpl(const SecureContextCheck privilegeContextCheck) const
 {
     if (!getSecurityOrigin()->isPotentiallyTrustworthy())
         return false;
 
     if (SchemeRegistry::schemeShouldBypassSecureContextCheck(getSecurityOrigin()->protocol()))
          return true;
  
      if (privilegeContextCheck == StandardSecureContextCheck) {
        Frame* parent = m_frame ? m_frame->tree().parent() : nullptr;
        if (parent && !parent->canHaveSecureChild())
            return false;
//         if (!m_frame)
//             return true;
//         Frame* parent = m_frame->tree().parent();
//         while (parent) {
//             if (!parent->securityContext()->getSecurityOrigin()->isPotentiallyTrustworthy())
//                 return false;
//             parent = parent->tree().parent();
//         }
      }
      return true;
  }