#ifndef LUA_RBOS_H
#define LUA_RBOS_H
// LuaRBOs.h: interface for the LuaRBOs class.
//
//////////////////////////////////////////////////////////////////////

#include <set>
using std::set;

#if !defined HEADLESS
#include "Rendering/GL/myGL.h"
#endif // !defined HEADLESS


struct lua_State;


class LuaRBOs {
	public:
		LuaRBOs();
		~LuaRBOs();

		static bool PushEntries(lua_State* L);

		struct RBO;
		static const RBO* GetLuaRBO(lua_State* L, int index);

	public:
		struct RBO {
			RBO() : id(0), target(0), format(0), xsize(0), ysize(0) {}
			void Init();
			void Free();

			GLuint id;
			GLenum target;
			GLenum format;
			GLsizei xsize;
			GLsizei ysize;
		};

	private:
		set<RBO*> rbos;

	private: // helpers
		static bool CreateMetatable(lua_State* L);

	private: // metatable methods
		static int meta_gc(lua_State* L);
		static int meta_index(lua_State* L);
		static int meta_newindex(lua_State* L);

	private:
		static int CreateRBO(lua_State* L);
		static int DeleteRBO(lua_State* L);
};


#endif /* LUA_RBOS_H */
