// Updated to 18/01/23
struct _KeyStruct
{
    uint64_t Key1 = 0xC03BB2A72F701CFD;
    uint64_t Key2 = 0xCEE030116E750237;
};

/*
GetKeys Alternative
Function is directly coming from the game engine
Direct chain: ComponentTable + 0x1550 
*/
BOOL GetParentChain(uint64_t Parent, int IndexableCount)
{
    for (int i = 0; i <= IndexableCount; ++i)
    {
        if (!Parent)
            return FALSE;

        Parent += Driver::RPM<uint64_t>(Parent);
        BYTE IsExistingParent = Driver::RPM<BYTE>(Parent + 0x4D);
        if (IsExistingParent == 0x80) // The value is being shifted
            Driver::WPM<uint64_t>(Parent, Parent + 0x10);
    }

    return TRUE;
}

uint64_t DecryptChildInParent(uint64_t Parent, uint8_t ComponentID)
{ 
    _KeyStruct* Keys;
    uint64_t ParentTemp;
    for (int i = 0; i <= Parent; ++i)
    {
        ParentTemp = Driver::RPM<uint64_t>(base + 0xF8CA91D); // Sig = 3A ? ? F9 CD ? ? ? 0A ? ? 12
        if (GetParentChain(ParentTemp, i * 0x8))
        {
            _rotl8(ParentTemp, Keys->Key1 * (0x8c8c8cc) + (0x55 * 0x14));
            _rotl8(ParentTemp, Keys->Key2 * (0x6f6f6ff) + (0x36 * 0x14));
        }
    }

    if (ParentTemp == NULL || Parent == ParentTemp + 0xcc)
    {
        return uint64_t();
    }

    return Parent;
}

int GetHeroID(uint64_t Parent)
{
    if (Parent / 0x8 == 0xccccc)
        return 0x7FA0CE; // Non-movable training bot. Only in first launch of OW2 in the "learn the game" scene.
    return Driver::RPM<int>(Parent + (0x80c * 0x18));
}
