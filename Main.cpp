#include <iostream>
#include <string>

extern "C"
{
#include "Lua540/include/lua.h"
#include "Lua540/include/lauxlib.h"
#include "Lua540/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "Lua540/liblua54.a")
#endif


bool CheckLua(lua_State *L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}

void run_string(lua_State *L, std::string cmd)
{

	if (CheckLua(L, luaL_dostring(L, cmd.c_str())))
	{
		// a will exist "globally within the LUA virtual machine.
		lua_getglobal(L, "a");  // note, LUA is "typeless"
		if (lua_isnumber(L, -1))
		{
			float a_in_cpp = (float)lua_tonumber(L, -1);
			std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
		}
	}

}

void run_file(lua_State *L, std::string filename)
{
	if (CheckLua(L, luaL_dofile(L, filename.c_str())))
	{
		// a will exist "globally within the LUA virtual machine.
		lua_getglobal(L, "a");  // note, LUA is "typeless"
		if (lua_isnumber(L, -1))
		{
			float a_in_cpp = (float)lua_tonumber(L, -1);
			std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
		}
	}

}

void setting_file_struct(lua_State *L)
{
	struct Player
	{
		std::string title;
		std::string name;
		std::string family;
		int level;
	} player;

	if (CheckLua(L, luaL_dofile(L, "Settings2.lua")))
	{
		lua_getglobal(L, "player");
		if (lua_istable(L, -1))  // is the top of the stack a table?
		{
			lua_pushstring(L, "Name"); // push string to search for 
			lua_gettable(L, -2); // ask table (now at -2) for result of search
			player.name = lua_tostring(L, -1); // extract result of search
			lua_pop(L, 1); // remove 1 item from top. (the extracted value)

			// end repeat for others.
			lua_pushstring(L, "Family"); // push string to search for 
			lua_gettable(L, -2); // ask table (now at -2) for result of search
			player.family = lua_tostring(L, -1); // extract result of search
			lua_pop(L, 1); // remove 1 item from top. (the extracted value)

			lua_pushstring(L, "Title"); // push string to search for 
			lua_gettable(L, -2); // ask table (now at -2) for result of search
			player.title = lua_tostring(L, -1); // extract result of search
			lua_pop(L, 1); // remove 1 item from top. (the extracted value)

			lua_pushstring(L, "Level"); // push string to search for 
			lua_gettable(L, -2); // ask table (now at -2) for result of search
			player.level = lua_tointeger(L, -1); // extract result of search
			lua_pop(L, 1); // remove 1 item from top. (the extracted value)
		}

		std::cout << player.name << std::endl;
		std::cout << player.family << std::endl;
		std::cout << player.title << std::endl;
		std::cout << player.level << std::endl;

	}

}

void setting_file_brute_force(lua_State *L)
{
	// brute force, load the settings.

	struct Player
	{
		std::string title;
		std::string name;
		std::string family;
		int level;
	} player;

	if (CheckLua(L, luaL_dofile(L, "Settings.lua")))
	{
		lua_getglobal(L, "PlayerName");
		if (lua_isstring(L, -1))
		{
			player.name = lua_tostring(L, -1);
		}
		lua_getglobal(L, "PlayerFamily");
		if (lua_isstring(L, -1))
		{
			player.family = lua_tostring(L, -1);
		}
		lua_getglobal(L, "PlayerTitle");
		if (lua_isstring(L, -1))
		{
			player.title = lua_tostring(L, -1);
		}
		lua_getglobal(L, "PlayerLevel");
		if (lua_isnumber(L, -1))
		{
			player.level = lua_tonumber(L, -1);
		}

		std::cout << player.name << std::endl;
		std::cout << player.family << std::endl;
		std::cout << player.title << std::endl;
		std::cout << player.level << std::endl;

	}
}

void calling_a_lua_function(lua_State *L)
{
	if (CheckLua(L, luaL_dofile(L, "Settings3.lua")))
	{
		lua_getglobal(L, "AddStuff");  // function is just a variable in lua.
		if (lua_isfunction(L, -1))
		{
			// so push two things onto the stack
			lua_pushnumber(L, 3.5f);
			lua_pushnumber(L, 7.1f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
				// VM, how many args in, how many args out, errorhandle
			{
				// note: LUA will have popped 2 items from stack.
				// also, no error checking here.
				std::cout << "[C++] called in Lua 'AddStuff(3.5f, 7.1f)', got " << (float)lua_tonumber(L, -1) << std::endl;
			}
		}
	}
}

void calling_getplayer(lua_State *L, int pn)
{
	struct Player
	{
		std::string title;
		std::string name;
		std::string family;
		int level;
	} player;

	if (CheckLua(L, luaL_dofile(L, "Settings3.lua")))
	{
		lua_getglobal(L, "GetPlayer");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, pn); // arg for function call
			if (CheckLua(L, lua_pcall(L, 1, 1, 0)))
			{
				if (lua_istable(L, -1))  // is the top of the stack a table?
				{
					lua_pushstring(L, "Name"); // push string to search for 
					lua_gettable(L, -2); // ask table (now at -2) for result of search
					player.name = lua_tostring(L, -1); // extract result of search
					lua_pop(L, 1); // remove 1 item from top. (the extracted value)

								   // end repeat for others.
					lua_pushstring(L, "Family"); // push string to search for 
					lua_gettable(L, -2); // ask table (now at -2) for result of search
					player.family = lua_tostring(L, -1); // extract result of search
					lua_pop(L, 1); // remove 1 item from top. (the extracted value)

					lua_pushstring(L, "Title"); // push string to search for 
					lua_gettable(L, -2); // ask table (now at -2) for result of search
					player.title = lua_tostring(L, -1); // extract result of search
					lua_pop(L, 1); // remove 1 item from top. (the extracted value)

					lua_pushstring(L, "Level"); // push string to search for 
					lua_gettable(L, -2); // ask table (now at -2) for result of search
					player.level = lua_tointeger(L, -1); // extract result of search
					lua_pop(L, 1); // remove 1 item from top. (the extracted value)
				}
				std::cout << player.name << std::endl;
				std::cout << player.family << std::endl;
				std::cout << player.title << std::endl;
				std::cout << player.level << std::endl;
			}

		}
	}

}

void calling_doathing(lua_State *L) 
{
	if (CheckLua(L, luaL_dofile(L, "Settings3.lua")))
	{
		lua_getglobal(L, "DoAThing");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);
			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "[C++] Called in LUA DoAThing(5.0f, 6.0f) got " << (float)lua_tonumber(L, -1) << std::endl;
			}
		}
	}

}

/*
callable from Lua
*/
int lua_HostFunction(lua_State *L)
{
	float a = (float)lua_tonumber(L, 1); // bottom of stack
	float b = (float)lua_tonumber(L, 2);
	std::cout << "[C++] HostFunction called with "<< a <<" and " << b << std::endl;
	float c = a * b;
	lua_pushnumber(L, c);
	return 1;
}

/*
main.
*/
int main()
{
	lua_State *L = luaL_newstate();   // create a LUA state

	std::cout << "---- from embedded strings ----" << std::endl;

	run_string(L, "a = 7 + 11");  // valid.
	run_string(L, "a = 7 + 11sdfskdj");  // invalid.

    // add the basic libs (including math) to the state machine.
	luaL_openlibs(L);
	run_string(L, "a = 7 + 11 + math.sin(23.7)");

	// values persist. "a" is still in the LUA VM.
	run_string(L, "a = a + 100");

	std::cout << "---- from file Example.lua ----" << std::endl;
	run_file(L, "Example.lua");

	std::cout << "---- from file Settings.lua ----" << std::endl;
	setting_file_brute_force(L);

	std::cout << "---- from file Settings2.lua ----" << std::endl;
	setting_file_struct(L);

	std::cout << "---- from file Settings3.lua ----" << std::endl;
	calling_a_lua_function(L);
	calling_getplayer(L, 0);
	calling_getplayer(L, 1);

	lua_register(L, "HostFunction", lua_HostFunction);

	calling_doathing(L);

	system("pause"); // so we can see the output when run from vsstudio
	lua_close(L);

	return 0;
}

/*

Link for tutorial: https://www.youtube.com/watch?v=4l5HdmPoynw&t=307s
Manual for LUA: https://www.lua.org/manual/5.4/manual.html

LUA Stack notes

LIFO, sorta.

LUA stack is indexed though. The first pushed item is at position 1. (not 0.)

Recall, from simple1, variable "a" can be anything, any type.
So the boxes in the stack can be any size, and will include type information.

Recall lua_getglobal() call. We "ask" for "a". In effect: 
 --> put "a" in stack, LUA pops it, replaces it with its a variable.

 LUA stack can be referenced relative from the top; -1 is the top, then -2, etc
 So we can interrogate that location as lua_isnumber(L, -1)

 When we did the invalid command, it put an error message on the top of the stack.

*/