/*
 *      Copyright (C) 2011 Hendrik Leppkes
 *      http://www.1f0.de
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  Initial design and concept by Gabest and the MPC-HC Team, copyright under GPLv2
 */

#include "stdafx.h"
#include "FontInstaller.h"

CFontInstaller::CFontInstaller()
{
	if(HMODULE hGdi = GetModuleHandle(_T("gdi32.dll"))) {
		pAddFontMemResourceEx = (HANDLE (WINAPI *)(PVOID,DWORD,PVOID,DWORD*))GetProcAddress(hGdi, "AddFontMemResourceEx");
		pRemoveFontMemResourceEx = (BOOL (WINAPI *)(HANDLE))GetProcAddress(hGdi, "RemoveFontMemResourceEx");
	}
}

CFontInstaller::~CFontInstaller()
{
	UninstallFonts();
}

bool CFontInstaller::InstallFont(const void* pData, UINT len)
{
	return InstallFontMemory(pData, len);
}

void CFontInstaller::UninstallFonts()
{
	if(pRemoveFontMemResourceEx) {
    std::vector<HANDLE>::iterator it;
    for(it = m_fonts.begin(); it != m_fonts.end(); ++it) {
      pRemoveFontMemResourceEx(*it);
    }
    m_fonts.empty();
	}
}

bool CFontInstaller::InstallFontMemory(const void* pData, UINT len)
{
	if(!pAddFontMemResourceEx) {
		return false;
	}

	DWORD nFonts = 0;
	HANDLE hFont = pAddFontMemResourceEx((PVOID)pData, len, NULL, &nFonts);
	if(hFont && nFonts > 0) {
		m_fonts.push_back(hFont);
	}
	return hFont && nFonts > 0;
}
