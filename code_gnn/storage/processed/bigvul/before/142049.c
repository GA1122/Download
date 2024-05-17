HRESULT CreateNewUser(OSUserManager* manager,
                      const wchar_t* base_username,
                      const wchar_t* password,
                      const wchar_t* fullname,
                      const wchar_t* comment,
                      bool add_to_users_group,
                      int max_attempts,
                      BSTR* final_username,
                      BSTR* sid) {
  DCHECK(manager);
  DCHECK(base_username);
  DCHECK(password);
  DCHECK(fullname);
  DCHECK(comment);
  DCHECK(final_username);
  DCHECK(sid);
  wchar_t new_username[kWindowsUsernameBufferLength];
  errno_t err = wcscpy_s(new_username, base::size(new_username), base_username);
  if (err != 0) {
    LOGFN(ERROR) << "wcscpy_s errno=" << err;
    return E_FAIL;
  }

  for (int i = 0; i < max_attempts; ++i) {
    CComBSTR new_sid;
    DWORD error;
    HRESULT hr = manager->AddUser(new_username, password, fullname, comment,
                                  add_to_users_group, &new_sid, &error);
    if (hr == HRESULT_FROM_WIN32(NERR_UserExists)) {
      base::string16 next_username = base_username;
      base::string16 next_username_suffix =
          base::NumberToString16(i + kInitialDuplicateUsernameIndex);
      if (next_username.size() + next_username_suffix.size() >
          (kWindowsUsernameBufferLength - 1)) {
        next_username =
            next_username.substr(0, (kWindowsUsernameBufferLength - 1) -
                                        next_username_suffix.size()) +
            next_username_suffix;
      } else {
        next_username += next_username_suffix;
      }
      LOGFN(INFO) << "Username '" << new_username
                  << "' already exists. Trying '" << next_username << "'";

      errno_t err = wcscpy_s(new_username, base::size(new_username),
                             next_username.c_str());
      if (err != 0) {
        LOGFN(ERROR) << "wcscpy_s errno=" << err;
        return E_FAIL;
      }

      continue;
    } else if (FAILED(hr)) {
      LOGFN(ERROR) << "manager->AddUser hr=" << putHR(hr);
      return hr;
    }

    *sid = ::SysAllocString(new_sid);
    *final_username = ::SysAllocString(new_username);
    return S_OK;
  }

  return HRESULT_FROM_WIN32(NERR_UserExists);
}
