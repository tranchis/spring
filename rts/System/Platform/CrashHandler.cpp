#include "CrashHandler.h"
#ifdef __APPLE__
#include <AvailabilityMacros.h>
#endif

#ifdef _WIN32
#include "Win/CrashHandler.h"
#include "Platform/Win/seh.h"

namespace CrashHandler {
	void Install()
	{
		Win32::Install();
		InitializeSEH();
	};
	
	void Remove()
	{
		Win32::Remove();
	}

	void InstallHangHandler()
	{
		Win32::InstallHangHandler();
	}

	void UninstallHangHandler()
	{
		Win32::UninstallHangHandler();
	}

	void ClearDrawWDT(bool disable) {
		Win32::ClearDrawWDT(disable);
	}

	void ClearSimWDT(bool disable) {
		Win32::ClearSimWDT(disable);
	}
};

#elif !defined(__APPLE__) || (MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4)

#include <string>
#include <vector>
#include <queue>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>

#include "LogOutput.h"
#include "errorhandler.h"
#include "Game/GameVersion.h"
#include "Platform/Misc.h"

namespace CrashHandler {
	
	void HandleSignal(int signal)
	{
		std::string error;
		std::queue<std::string> addresses;
		logOutput.RemoveAllSubscribers();
		{
			LogObject log;
			if (signal == SIGSEGV)
				error = "Segmentation fault (SIGSEGV)";
			else if (signal == SIGILL)
				error = "Illegal instruction (SIGILL)";
			else if (signal == SIGPIPE)
				error = "Broken pipe (SIGPIPE)";
			else if (signal == SIGIO)
				error = "IO-Error (SIGIO)";
			else if (signal == SIGABRT)
				error = "Aborted (SIGABRT)";
			log << error << " in spring " << SpringVersion::GetFull() << "\nStacktrace:\n";

			std::vector<void*> buffer(128);
			const int numptr = backtrace(&buffer[0], buffer.size()); // stack pointers
			char ** strings = backtrace_symbols(&buffer[0], numptr); // give them meaningfull names
			if (strings == NULL)
			{
				log << "Unable to create stacktrace\n";
			}
			else
			{
				for (int j = 0; j < numptr; j++)
				{
					std::string addrline(strings[j]);
					log << addrline << "\n";
					size_t begin = addrline.find_first_of('[');
					if (begin != std::string::npos)
					{
						size_t end = addrline.find_first_of(']');
						if (end != std::string::npos)
						{
							addresses.push(addrline.substr(begin+1, end-begin-1));
						}
					}
				}
				delete strings;
			}
		}
		logOutput.End();  // Stop writing to log.

		std::ostringstream buf;
		buf << "addr2line " << "--exe=\"" << Platform::GetBinaryFile() << "\"";
		while (!addresses.empty())
		{
			buf << " " << addresses.front();
			addresses.pop();
		}
		buf << " >> infolog.txt"; // pipe to infolog (which will be in CWD)
		system(buf.str().c_str());
		ErrorMessageBox(error, "Spring crashed", 0);
	};
	
	void Install()
	{
		signal(SIGSEGV, HandleSignal); // segmentation fault
		signal(SIGILL, HandleSignal); // illegal instruction
		signal(SIGPIPE, HandleSignal); // sigpipe, maybe some network error
		signal(SIGIO, HandleSignal); // who knows?
		signal(SIGABRT, HandleSignal);
	};
	
	void Remove()
	{
		signal(SIGSEGV, SIG_DFL);
		signal(SIGILL, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		signal(SIGIO, SIG_DFL);
		signal(SIGABRT, SIG_DFL);
	}

	void InstallHangHandler() {
	}
	
	void UninstallHangHandler() {
	}

	void ClearDrawWDT(bool disable) {
	}

	void ClearSimWDT(bool disable) {
	}
};

#else

namespace CrashHandler {
	void HandleSignal(int signal){};
	void Install(){};
	void Remove(){}
	void InstallHangHandler() {}	
	void UninstallHangHandler() {}
	void ClearDrawWDT(bool disable) {}
	void ClearSimWDT(bool disable) {}
};

#endif