int MacroExpander::expandEscapedMacro(const QString &str, int pos, QStringList &ret)
{
    ushort option = str[pos+1].unicode();

    switch (option) {
    case 'f':  
    case 'F':  
        if (m_device.is<Solid::StorageAccess>()) {
            ret << m_device.as<Solid::StorageAccess>()->filePath();
        } else {
            qWarning() << "DeviceServiceAction::execute: " << m_device.udi()
                       << " is not a StorageAccess device";
        }
        break;
    case 'd':  
    case 'D':  
        if (m_device.is<Solid::Block>()) {
            ret << m_device.as<Solid::Block>()->device();
        } else {
            qWarning() << "DeviceServiceAction::execute: " << m_device.udi()
                       << " is not a Block device";
        }
        break;
    case 'i':  
    case 'I':  
        ret << m_device.udi();
        break;
    case '%':
        ret = QStringList(QLatin1String("%"));
        break;
    default:
        return -2;  
    }
    return 2;
}
