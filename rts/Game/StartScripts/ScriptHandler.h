#ifndef SCRIPTHANDLER_H
#define SCRIPTHANDLER_H
// ScriptHandler.h: interface for the CScriptHandler class.
//
//////////////////////////////////////////////////////////////////////

#include <string>
#include <map>
#include <list>
#include "Script.h"
#if !defined HEADLESS
#include "Rendering/GL/glList.h"
#endif // !defined HEADLESS

class CScriptHandler
{
public:
	static void SelectScript(std::string s);
#if !defined HEADLESS
	CglList* GenList(ListSelectCallback callb);
#endif // !defined HEADLESS

	void AddScript(std::string name,CScript* s);
	void LoadScriptFile(const std::string& file);

	static CScriptHandler& Instance();

	CScript* chosenScript;  ///< Pointer to the selected CScript.
	std::string chosenName; ///< Name of the selected script.
private:
	std::map<std::string,CScript*> scripts; ///< Maps script names to CScript pointers.
	std::list<CScript*> loaded_scripts;     ///< Scripts loaded and owned by CScriptHandler
#if !defined HEADLESS
	static ListSelectCallback callback;
#endif // !defined HEADLESS
	CScriptHandler();
	CScriptHandler(CScriptHandler const&);
	CScriptHandler& operator=(CScriptHandler const&);
	void LoadScripts();
	~CScriptHandler();
};

#endif /* SCRIPTHANDLER_H */
