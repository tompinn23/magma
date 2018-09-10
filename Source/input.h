#pragma once

#include "SDL.h"
#include "spdlog/spdlog.h"

#include <vector>
#include <map>
#include <stack>
#include <set>


namespace magma {
	//Make sure this is updated you goddamn moron.
	//Make sure this is updated you goddamn moron.
	// ALSO INVALID DOES'NT COUNT.
	const int domainNum = 3;
	enum inputDomains : int {
		INPUT_DOMAIN_INVALID = -1,
		INPUT_DOMAIN_DEFAULT,
		INPUT_DOMAIN_TEXTBOX,
		INPUT_DOMAIN_UI,
		INPUT_DOMAIN_TEST
	};
	const inputDomains inputDomainList[] {
		INPUT_DOMAIN_DEFAULT,
		INPUT_DOMAIN_TEXTBOX,
		INPUT_DOMAIN_UI,
		INPUT_DOMAIN_TEST,
	};

	enum commands : int {
		CMD_RESERVE_K = 31,
		CMD_SPACE_K,
		CMD_EXCL_K,
		CMD_QUOTE_K,
		CMD_HASH_K,
		CMD_DOLLAR_K,
		CMD_PERCENT_K,
		CMD_AMPERSAND_K,
		CMD_APOSTROPHE_K,
		CMD_LEFT_PAREN_K,
		CMD_RIGHT_PAREN_K,
		CMD_ASTERIX_K,
		CMD_PLUS_K,
		CMD_COMMA_K,
		CMD_MINUS_K,
		CMD_STOP_K,
		CMD_SLASH_K,
		CMD_0_K,
		CMD_1_K,
		CMD_2_K,
		CMD_3_K,
		CMD_4_K,
		CMD_5_K,
		CMD_6_K,
		CMD_7_K,
		CMD_8_K,
		CMD_9_K,
		CMD_COLON_K,
		CMD_SEMICOLON_K,
		CMD_LT_K,
		CMD_EQUALS_K,
		CMD_GT_K,
		CMD_QUESTION_K,
		CMD_AT_K,
		CMD_A_K,
		CMD_B_K,
		CMD_C_K,
		CMD_D_K,
		CMD_E_K,
		CMD_F_K,
		CMD_G_K,
		CMD_H_K,
		CMD_I_K,
		CMD_J_K,
		CMD_K_K,
		CMD_L_K,
		CMD_M_K,
		CMD_N_K,
		CMD_O_K,
		CMD_P_K,
		CMD_Q_K,
		CMD_R_K,
		CMD_S_K,
		CMD_T_K,
		CMD_U_K,
		CMD_V_K,
		CMD_W_K,
		CMD_X_K,
		CMD_Y_K,
		CMD_Z_K,
		CMD_L_SQ_K,
		CMD_B_SLASH_K,
		CMD_R_SQ_K,
		CMD_CARET_K,
		CMD_UNDERSCORE_K,
		CMD_GRAVE_K,
		CMD_a_K,
		CMD_b_K,
		CMD_c_K,
		CMD_d_K,
		CMD_e_K,
		CMD_f_K,
		CMD_g_K,
		CMD_h_K,
		CMD_i_K,
		CMD_j_K,
		CMD_k_K,
		CMD_l_K,
		CMD_m_K,
		CMD_n_K,
		CMD_o_K,
		CMD_p_K,
		CMD_q_K,
		CMD_r_K,
		CMD_s_K,
		CMD_t_K,
		CMD_u_K,
		CMD_v_K,
		CMD_w_K,
		CMD_x_K,
		CMD_y_K,
		CMD_z_K,
		CMD_L_BRACKET_K,
		CMD_PIPE_K,
		CMD_R_BRACKET_K,
		CMD_TILDA_K = 126,
		CMD_BACKSPACE,
		CMD_ENTER,
		CMD_MOVE_N,
		CMD_MOVE_E,
		CMD_MOVE_NE,
		CMD_MOVE_W,
		CMD_MOVE_NW,
		CMD_MOVE_S,
		CMD_MOVE_SW,
		CMD_MOVE_SE,
		CMD_HIT,

	};

	class CommandDef {
	public:
		CommandDef(commands cmd, std::string name, bool iskb, SDL_Keycode key, bool pressed, bool shift, bool ctrl, bool alt, bool cpslk);
		bool same_activator(SDL_Keycode key, bool pressed, bool shift, bool ctrl, bool alt, bool cpslk);
		commands getCmd() { return cmd; }
		std::string getName() { return name;}
		SDL_Keycode getKey() { return key; }
	private:
		commands cmd;
		std::string name;
		SDL_Keycode key;
		bool iskb, shift, ctrl, alt, cpslk, pressed;
	};

	class CommandManager {
	public:
		CommandManager();
		void addCommand(inputDomains domain, CommandDef command);
		void activateDomain(inputDomains dom);
		void deactivateDomain(inputDomains dom);
		void singleDomain(inputDomains dom) { singleDomainData = dom; }
		void stopSingle() { singleDomainData = INPUT_DOMAIN_INVALID; }
		int getCommand(commands& cmd, std::string& name);
		int getCommand(commands& cmd);
		void checkCommand(Uint32 type, SDL_Event& evt);
		void updateMouse(int x, int y);
	private:
		inputDomains singleDomainData = INPUT_DOMAIN_INVALID;
		std::set<inputDomains> activeDomains;
		std::stack<std::string> to_be_processed_names;
		std::stack<commands> to_be_processed;
		std::vector<std::multimap<SDL_Keycode, CommandDef>> cmds;
		std::shared_ptr<spdlog::logger> mLog;
	};

}
