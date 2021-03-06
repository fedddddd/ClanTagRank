#include <stdinc.hpp>

namespace command
{
	std::unordered_map<std::string, std::function<void(params&)>> handlers;

	void main_handler()
	{
		params params = {};

		const auto command = utils::string::to_lower(params[0]);

		if (handlers.find(command) != handlers.end())
		{
			handlers[command](params);
		}
	}

	int params::size()
	{
		return game::Cmd_Argc();
	}

	const char* params::get(int index)
	{
		return game::Cmd_Argv(index);
	}

	std::string params::join(int index)
	{
		std::string result = {};

		for (int i = index; i < this->size(); i++)
		{
			if (i > index) result.append(" ");
			result.append(this->get(i));
		}

		return result;
	}

	void add_raw(const char* name, void (*callback)())
	{
		game::Cmd_AddCommandInternal(name, callback, utils::memory::get_allocator()->allocate<game::cmd_function_t>());
	}

	void add(const char* name, std::function<void(params&)> callback)
	{
		const auto command = utils::string::to_lower(name);

		if (handlers.find(command) == handlers.end())
		{
			add_raw(name, main_handler);
		}

		handlers[command] = callback;
	}

    void init()
    {
        command::add("tellraw", [](command::params& params)
        {
            if (params.size() < 3)
            {
                return;
            }

            const auto client = atoi(params.get(1));
            const auto message = params.join(2);

            game::SV_GameSendServerCommand(client, 0, utils::string::va("%c \"%s\"",
                SELECT_VALUE(106, 106, 84), message.data()));

            printf("tell %i -> %s\n", client, message.data());
        });

        command::add("sayraw", [](command::params& params)
        {
            if (params.size() < 2)
            {
                return;
            }

            const auto message = params.join(1);

            game::SV_GameSendServerCommand(-1, 0, utils::string::va("%c \"%s\"",
                SELECT_VALUE(106, 106, 84), message.data()));

            printf("say -> %s\n", message.data());
        });

        command::add("clientprint", [](command::params& params)
        {
            if (params.size() < 3)
            {
                return;
            }

            const auto client = atoi(params.get(1));
            const auto message = params.join(2);

            game::SV_GameSendServerCommand(client, 0, utils::string::va("%c \"%s\"",
                SELECT_VALUE(59, 59, 102), message.data()));

            printf("print %i -> %s\n", client, message.data());
        });

        command::add("allclientsprint", [](command::params& params)
        {
            if (params.size() < 2)
            {
                return;
            }

            const auto message = params.join(1);

            game::SV_GameSendServerCommand(-1, 0, utils::string::va("%c \"%s\"",
                SELECT_VALUE(59, 59, 102), message.data()));

            printf("print -> %s\n", message.data());
        });

        command::add("clientprintbold", [](command::params& params)
        {
            if (params.size() < 3)
            {
                return;
            }

            const auto client = atoi(params.get(1));
            const auto message = params.join(2);

            game::SV_GameSendServerCommand(client, 0, utils::string::va("%c \"%s\"",
                SELECT_VALUE(60, 60, 99), message.data()));

            printf("printbold %i -> %s\n", client, message.data());
        });

        command::add("allclientsprintbold", [](command::params& params)
        {
            if (params.size() < 2)
            {
                return;
            }

            const auto message = params.join(1);

            game::SV_GameSendServerCommand(-1, 0, utils::string::va("%c \"%s\"",
                SELECT_VALUE(60, 60, 99), message.data()));

            printf("printbold -> %s\n", message.data());
        });
    }
}