#pragma once
#include "Magma.h"
#include <string>


namespace magma {

	MAGMA_DECL std::string get_error(std::string err);
	MAGMA_DECL void clear_error();
	int set_error(std::string err);

}