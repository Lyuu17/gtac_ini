# gtac_ini

INI module for GTA Connected

TODO:

 * MultiLine
 * MultiKey
 * MultiValues

Script example:
 ```
 	ptr = module.INI.Create();

 	ptr.LoadFile("abc.txt")

 	ptr.SetString("Section", "Test", "Hello world!", "Comment example");

 	ptr.SaveFile("abc.txt")

 	module.INI.Delete(ptr);
 ```

Functions available:
 ```
 * ptr.Errno(int errid)
 * ptr.Reset();
 * ptr.Delete(string szSection, string szKey, bool bRemoveEmpty = true);
 * ptr.IsEmpty();
 * ptr.IsMultiKey();
 * ptr.IsMultiLine();
 * ptr.IsUnicode();
 * ptr.IsUsingSpaces()
 * ptr.SetUnicode(bool bUnicode)
 * ptr.LoadFile(string szFile)
 * ptr.SaveFile(string szFile, bool bSign)
 * ptr.GetBoolValue(string szSection, string szKey, bool bDefaultValue = false)
 * ptr.GetIntValue(string szSection, string szKey, bool iDefaultValue = 0)
 * ptr.GetFloatValue(string szSection, string szKey, bool fDefaultValue = 0.0)
 * ptr.GetString(string szSection, string szKey, string szDefaultValue = "")
 * ptr.SetBoolValue(string szSection, string szKey, bool bValue, string szComment, bool bForceReplace)
 * ptr.SetIntValue(string szSection, string szKey, int iValue, string szComment, bool bUseHex, bool bForceReplace)
 * ptr.SetFloatValue(string szSection, string szKey, float fValue, string szComment, bool bReplace)
 * ptr.SetString(string szSection, string szKey, string szValue, string szComment, bool bForceReplace)
 ```