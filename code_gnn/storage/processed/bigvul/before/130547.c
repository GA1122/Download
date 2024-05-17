static void showUnderInvalidationError(const char* messagePrefix, const char* reason, const DisplayItem* newItem, const DisplayItem* oldItem)
{
#ifndef NDEBUG
    WTFLogAlways("%s %s:\nNew display item: %s\nOld display item: %s\nSee http://crbug.com/450725.", messagePrefix, reason,
        newItem ? newItem->asDebugString().utf8().data() : "None",
        oldItem ? oldItem->asDebugString().utf8().data() : "None");
#else
    WTFLogAlways("%s %s. Run debug build to get more details\nSee http://crbug.com/450725.", messagePrefix, reason);
#endif  
}
