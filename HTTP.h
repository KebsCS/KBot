#ifndef _HTTP_H_
#define _HTTP_H_

#include<Windows.h>
#include<WinInet.h>
#pragma comment(lib,"WinInet.lib")
#include <string>

class HTTP
{
private:

	std::string GetHost(std::string url)
	{
		bool Ishttps;
		url.find("https://") == std::string::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find("https://") + strlen("https://") : strpos = url.find("http://") + strlen("http://");
		url.find('/', strpos) == std::string::npos ? coutof = url.find('/', strpos) : coutof = url.find('/', strpos);
		std::string host;
		if (Ishttps)host = url.substr(strpos, coutof - strlen("https://"));
		else host = url.substr(strpos, coutof - strlen("http://"));
		strpos = host.find(':');
		if (strpos != std::wstring::npos)host = host.substr(0, strpos);
		return host;
	}

public:

	//https://developer.riotgames.com/docs/lol#game-client-api_live-client-data-api
	//example: GET("https://127.0.0.1", "/liveclientdata/allgamedata", 2999)
	std::string GET(std::string url, std::string params = "", int port = -1)
	{
		std::string ResultData;
		bool IsHttps;
		url.find("https") == std::string::npos ? IsHttps = false : IsHttps = true;
		HINTERNET hOpen = InternetOpenA("Mozilla Firefox/5.0 (Windows NT 6.1; Win64; x64; rv:47.0)", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		if (!hOpen)
		{
			return "InternetOpenA Error";
		}
		INTERNET_PORT InterPort;
		if (port == -1)
			IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		else
			InterPort = port;

		HINTERNET hConnect = InternetConnectA(hOpen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, NULL);
		if (!hConnect)
		{
			InternetCloseHandle(hOpen);
			return "InternetConnectA Error";
		}

		DWORD RequestFlg = INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
			INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
			INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
			INTERNET_FLAG_NO_AUTH |
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_NO_COOKIES |
			INTERNET_FLAG_NO_UI |
			INTERNET_FLAG_PRAGMA_NOCACHE |
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE;

		HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", params.c_str(), (LPCSTR)HTTP_VERSION, NULL, NULL, RequestFlg, NULL);
		if (!hRequest)
		{
			InternetCloseHandle(hConnect); InternetCloseHandle(hOpen);
			return "HttpOpenRequestA Error";
		}

		// ignores ssl certificate error
		DWORD dwFlags;
		DWORD dwBuffLen = sizeof(dwFlags);
		if (InternetQueryOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, &dwBuffLen))
		{
			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_REVOCATION;
			InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
		}

		BOOL status = HttpSendRequestA(hRequest, NULL, 0, nullptr, NULL);
		if (!status)
		{
			InternetCloseHandle(hConnect); InternetCloseHandle(hOpen); InternetCloseHandle(hRequest);
			return "HttpSendRequestA Error";
		}

		char* pResultData = nullptr;
		try { pResultData = new char[1025]; }
		catch (...) { InternetCloseHandle(hConnect); InternetCloseHandle(hOpen); InternetCloseHandle(hRequest); return "Error 1"; }
		UINT  ResultLen = 0;
		do
		{
			ZeroMemory(pResultData, 1025);
			InternetReadFile(hRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			ResultData.append(reinterpret_cast<char*>(pResultData), ResultLen);
		} while (ResultLen);
		char* pTmpQuery = nullptr;
		try { pTmpQuery = new char[4096]; }
		catch (...) { InternetCloseHandle(hConnect); InternetCloseHandle(hOpen); InternetCloseHandle(hRequest); return "Error 2"; }
		ZeroMemory(pTmpQuery, 4096 * sizeof(char));
		InternetCloseHandle(hOpen);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		return ResultData;
	}
};

#endif //!_HTTP_H_