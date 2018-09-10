#include "input.h"

namespace magma {

	CommandDef::CommandDef(commands cmd, std::string name, bool iskb, SDL_Keycode key, bool pressed, bool shift, bool ctrl, bool alt, bool cpslk) :
		cmd(cmd), name(name), iskb(iskb), key(key), pressed(pressed), shift(shift), ctrl(ctrl), alt(alt), cpslk(cpslk)
	{
	}

	bool CommandDef::same_activator(SDL_Keycode o_key, bool o_pressed, bool o_shift, bool o_ctrl, bool o_alt, bool o_cpslk)
	{
		if (o_key == SDLK_BACKSPACE && key == o_key && o_pressed == pressed || o_key == SDLK_SPACE && key == o_key && o_pressed == pressed || o_key == SDLK_RETURN && key == o_key && o_pressed == pressed)
			return true;
		return o_key == key && o_pressed == pressed && o_shift == shift && o_ctrl == ctrl && o_alt == alt && o_cpslk == cpslk;
	}

	CommandManager::CommandManager()
	{
		mLog = spdlog::get("main");
		cmds.resize(domainNum);
	}

	void CommandManager::addCommand(inputDomains domain, CommandDef command)
	{
		cmds[domain].insert(std::make_pair(command.getKey(), command));
	}

	void CommandManager::activateDomain(inputDomains dom)
	{
		activeDomains.insert(dom);
	}

	void CommandManager::deactivateDomain(inputDomains dom)
	{
		activeDomains.erase(dom);
	}

	int CommandManager::getCommand(commands & cmd)
	{
		if (to_be_processed.empty())
			return 0;
		cmd = to_be_processed.top();
		to_be_processed.pop();
		return 1;
	}
	int CommandManager::getCommand(commands & cmd, std::string& name)
	{
		if (to_be_processed.empty())
			return 0;
		cmd = to_be_processed.top();
		to_be_processed.pop();
		name = to_be_processed_names.top();
		to_be_processed_names.pop();
		return 1;
	}

	void CommandManager::checkCommand(Uint32 type, SDL_Event & evt)
	{
		bool shift = false;
		bool ctrl = false;
		bool pressed = false;
		bool alt = false;
		bool caps = false;
		switch (type)
		{
		case SDL_KEYDOWN:
			pressed = true;
			break;
		case SDL_KEYUP:
			pressed = false;
			break;
		}
		if (KMOD_CTRL & evt.key.keysym.mod)
			ctrl = true;
		if (KMOD_SHIFT & evt.key.keysym.mod)
			shift = true;
		if (KMOD_ALT & evt.key.keysym.mod)
			alt = true;
		if (KMOD_CAPS & evt.key.keysym.mod)
			caps = true;
		if (singleDomainData == INPUT_DOMAIN_INVALID)
		{
			for (auto& domain : activeDomains)
			{
				auto found = cmds[domain].equal_range(evt.key.keysym.sym);
				for (auto it = found.first; it != found.second; ++it)
				{
					if (it->second.same_activator(evt.key.keysym.sym, pressed, shift, ctrl, alt, caps))
					{
						to_be_processed.push(it->second.getCmd());
						to_be_processed_names.push(it->second.getName());
					}
				}
			}
		}
		else {
			auto found = cmds[singleDomainData].equal_range(evt.key.keysym.sym);
			for (auto it = found.first; it != found.second; ++it)
			{
				if (it->second.same_activator(evt.key.keysym.sym, pressed, shift, ctrl, alt, caps))
				{
					to_be_processed.push(it->second.getCmd());
					to_be_processed_names.push(it->second.getName());
				}
			}
		
		}
	}

}
