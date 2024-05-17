HandleDNSConfigMessage(const dns_cfg_message_t *msg, undo_lists_t *lists)
{
    DWORD err = 0;
    wchar_t addr[46];  
    undo_type_t undo_type = (msg->family == AF_INET6) ? undo_dns4 : undo_dns6;
    int addr_len = msg->addr_len;

     
    if (addr_len > _countof(msg->addr))
    {
        addr_len = _countof(msg->addr);
    }

    if (!msg->iface.name[0])  
    {
        return ERROR_MESSAGE_DATA;
    }

    wchar_t *wide_name = utf8to16(msg->iface.name);  
    if (!wide_name)
    {
        return ERROR_OUTOFMEMORY;
    }

     
    if (addr_len > 0 || msg->header.type == msg_del_dns_cfg)
    {
        err = DeleteDNS(msg->family, wide_name);
        if (err)
        {
            goto out;
        }
        free(RemoveListItem(&(*lists)[undo_type], CmpWString, wide_name));
    }

    if (msg->header.type == msg_del_dns_cfg)  
    {
        goto out;
    }

    for (int i = 0; i < addr_len; ++i)
    {
        if (msg->family == AF_INET6)
        {
            RtlIpv6AddressToStringW(&msg->addr[i].ipv6, addr);
        }
        else
        {
            RtlIpv4AddressToStringW(&msg->addr[i].ipv4, addr);
        }
        err = AddDNS(msg->family, wide_name, addr);
        if (i == 0 && err)
        {
            goto out;
        }
         
    }

    if (msg->addr_len > 0)
    {
        wchar_t *tmp_name = wcsdup(wide_name);
        if (!tmp_name || AddListItem(&(*lists)[undo_type], tmp_name))
        {
            free(tmp_name);
            DeleteDNS(msg->family, wide_name);
            err = ERROR_OUTOFMEMORY;
            goto out;
        }
    }

    err = 0;

out:
    free(wide_name);
    return err;
}