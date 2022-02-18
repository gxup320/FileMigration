#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

int main()
{
    TCHAR source[MAX_PATH] = { 0 };
    TCHAR target[MAX_PATH] = { 0 };
    _tprintf(_T("input source:"));
    _fgetts(source, MAX_PATH, stdin);
    source[_tcslen(source) - 1] = 0;
    _tprintf(_T("input target:"));
    _fgetts(target, MAX_PATH, stdin);
    target[_tcslen(target) - 1] = 0;
    DWORD attributes = GetFileAttributes(source);
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        DWORD ErrorCode = GetLastError();
        _tprintf(_T("Get source attributes fail, Code %d,0x%X\n"), ErrorCode, ErrorCode);
        _tprintf(_T("source:%s\n"), source);
        return ErrorCode;
    }
    _tprintf(_T("Moving occupied files will result in file corruption, continue? [Y/N]:"));
    TCHAR test = 0;
    _tscanf(_T("%c"), &test);
    if (test != _T('Y') && test != _T('y'))
    {
        _tprintf(_T("operation canceled\n"));
        return 0;
    }
    //move files
    _tprintf(_T("move file(s)... "));
    SHFILEOPSTRUCT mv = { 0 };
    mv.hwnd = GetConsoleWindow();
    mv.wFunc = FO_MOVE;
    mv.pFrom = source;
    mv.pTo = target;
    mv.fFlags = FOF_NOCONFIRMMKDIR;
    int recode = SHFileOperation(&mv);
    if (recode == 0)
    {
        _tprintf(_T("success\n"));
    }
    else
    {
        _tprintf(_T("fail, Code %d,0x%X\n"), recode, recode);
        return recode;
    }
    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        //directory
        if (CreateSymbolicLink(source, target, SYMBOLIC_LINK_FLAG_DIRECTORY))
        {
            _tprintf(_T("Create symbolic link success\n"));
        }
        else
        {
            DWORD ErrorCode = GetLastError();
            _tprintf(_T("Create symbolic link fail, Code %d,0x%X\n"), ErrorCode, ErrorCode);
            return ErrorCode;
        }
    }
    else
    {
        //file
        if (CreateSymbolicLink(source, target, NULL))
        {
            _tprintf(_T("Create symbolic link success\n"));
        }
        else
        {
            DWORD ErrorCode = GetLastError();
            _tprintf(_T("Create symbolic link fail, Code %d,0x%X\n"), ErrorCode, ErrorCode);
            return ErrorCode;
        }
    }
}
