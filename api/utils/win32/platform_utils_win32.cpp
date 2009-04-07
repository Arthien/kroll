/**
 * Appcelerator Kroll - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#include "../utils.h"
#include "../poco/UnWindows.h"

namespace kroll
{
	void PlatformUtils::GetNodeId(NodeId& id)
	{
		PIP_ADAPTER_INFO pAdapterInfo;
		PIP_ADAPTER_INFO pAdapter = 0;
		ULONG len    = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into len
		DWORD rc = GetAdaptersInfo(pAdapterInfo, &len);
		if (rc == ERROR_BUFFER_OVERFLOW) 
		{
			delete [] reinterpret_cast<char*>(pAdapterInfo);
			pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
		}
		else if (rc != ERROR_SUCCESS)
		{
			throw std::string("cannot get network adapter list");
		}
		try
		{
			bool found = false;
			if (GetAdaptersInfo(pAdapterInfo, &len) == NO_ERROR) 
			{
				pAdapter = pAdapterInfo;
				while (pAdapter && !found) 
				{
					if (pAdapter->Type == MIB_IF_TYPE_ETHERNET && pAdapter->AddressLength == sizeof(id))
					{
						std::memcpy(&id, pAdapter->Address, pAdapter->AddressLength);
						found = true;
					}
					pAdapter = pAdapter->Next;
				}
			}
			else throw std::string("cannot get network adapter list");
			if (!found) throw std::string("no Ethernet adapter found");
		}
		catch (Exception&)
		{
			delete [] reinterpret_cast<char*>(pAdapterInfo);
			throw;
		}
		delete [] reinterpret_cast<char*>(pAdapterInfo);
	}
}
