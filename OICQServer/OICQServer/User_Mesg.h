#pragma once
struct User
{
	CString username;
	CString password;
	bool alive = false;
};
struct Mesg
{
	wchar_t from[20] = { 0 };
	wchar_t to[20] = { 0 };
	int length;
	wchar_t content[200] = { 0 };
};