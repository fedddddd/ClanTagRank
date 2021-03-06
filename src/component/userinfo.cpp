#include <stdinc.hpp>

namespace
{
	std::string clantags[18];
	std::string names[18];

	int client_index;

	const char* get_clantag(int clientNum, const char* s, const char* key)
	{
		if (clantags[clientNum].empty())
		{
			return game::Info_ValueForKey(s, key);
		}

		return clantags[clientNum].data();
	}

	const char* get_name(int clientNum, const char* s, const char* key)
	{
		if (names[clientNum].empty())
		{
			return game::Info_ValueForKey(s, key);
		}

		return names[clientNum].data();
	}

	void sv_get_user_info_stub(int index, char* buffer, int bufferSize)
	{
		client_index = index;

		game::SV_GetUserInfo(index, buffer, bufferSize);
	}

	const char* info_value_for_name(const char* s, const char* key)
	{
		const auto name = get_name(client_index, s, key);

		return name;
	}

	const char* info_value_for_clantag(const char* s, const char* key)
	{
		const auto name = get_clantag(client_index, s, key);

		return name;
	}

	void client_disconnect_stub(int clientNum)
	{
		clantags[clientNum].clear();
		names[clientNum].clear();

		reinterpret_cast<void (*)(int)>(SELECT_VALUE(0x42FB00, 0x64ADE0, 0x4FB560))(clientNum);
	}

	const char* using_tag_hook(const char*, const char*)
	{
		return "1";
	}
}

namespace userinfo
{
	void init()
	{
		utils::hook::call(SELECT_VALUE(0x4ED794, 0x427EB4, 0x4FAE81), info_value_for_name);
		utils::hook::call(SELECT_VALUE(0x4ED7BD, 0x427EDD, 0x4FAEAD), info_value_for_clantag);

		utils::hook::call(SELECT_VALUE(0x4ED6D0, 0x427DF0, 0x4FADD8), sv_get_user_info_stub);

		utils::hook::call(SELECT_VALUE(0x4D8888, 0x5304C8, 0x570735), client_disconnect_stub);

		if (game::is_iw5())
		{
			utils::hook::call(0x4FAEF8, using_tag_hook);
		}

		command::add("setclantag", [](command::params& params)
		{
			if (params.size() < 3)
			{
				printf("Usage: setclantag <num> <clantag>\n");
				return;
			}

			const auto client = params.get(1);
			const auto clantag = params.get(2);

			const auto clientNum = atoi(client);
			const auto sv_maxclients = game::get_dvar_int("sv_maxclients");

			if (clientNum >= sv_maxclients)
			{
				return;
			}

			char clean_tag[8] = { 0 };
			strncpy_s(clean_tag, clantag, 7);
			game::I_CleanStr(clean_tag);

			clantags[clientNum] = clean_tag;
			game::ClientUserInfoChanged(clientNum);
		});

		command::add("setclantagraw", [](command::params& params)
		{
			if (params.size() < 3)
			{
				printf("Usage: setclantagraw <num> <clantag>\n");
				return;
			}

			const auto client = params.get(1);
			auto clantag = params.get(2);

			const auto clientNum = atoi(client);
			auto sv_maxclients = game::get_dvar_int("sv_maxclients");

			if (clientNum >= sv_maxclients)
			{
				return;
			}

			clantags[clientNum] = clantag;
			game::ClientUserInfoChanged(clientNum);
		});

		command::add("rename", [](command::params& params)
		{
			if (params.size() < 3)
			{
				printf("Usage: rename <num> <name>\n");
				return;
			}

			const auto client = params.get(1);
			const auto name = params.get(2);

			const auto clientNum = atoi(client);
			const auto sv_maxclients = game::get_dvar_int("sv_maxclients");

			if (clientNum >= sv_maxclients)
			{
				return;
			}

			names[clientNum] = name;
			game::ClientUserInfoChanged(clientNum);
		});

		command::add("resetname", [](command::params& params)
		{
			if (params.size() < 2)
			{
				printf("Usage: resetname <num>\n");
				return;
			}

			const auto client = params.get(1);

			const auto clientNum = atoi(client);
			const auto sv_maxclients = game::get_dvar_int("sv_maxclients");

			if (clientNum >= sv_maxclients)
			{
				return;
			}

			names[clientNum].clear();
			game::ClientUserInfoChanged(clientNum);
		});
	}
}