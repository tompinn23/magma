#include "Error.h"

#include <mutex>

namespace magma {
	std::mutex mtx;
	static std::string last_err;

	
	std::string get_error()
	{
		std::string err;
		mtx.lock();
		err = last_err;
		mtx.unlock();
		return err;
	}

	void clear_error()
	{
		last_err = "";
	}
	int set_error(std::string err)
	{
		mtx.lock();
		last_err = err;
		mtx.unlock();
		return 1;
	}

}