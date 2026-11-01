#ifndef COMPONENT_TOOLS_RP_H_
#define COMPONENT_TOOLS_RP_H_

#include "../uCosmos/uCosmos.h"

namespace tools_rp {
	void reset_cmd(int argc, char * argv[]);
	void mem_cmd(int argc, char * argv[]);
	void fcpu_cmd(int argc, char * argv[]);

	void gpio_dir_cmd(int argc, char * argv[]);
	void gpio_out_cmd(int argc, char * argv[]);
	void gpio_in_cmd(int argc, char * argv[]);
	void test_cmd(int argc, char * argv[]);

}

#endif
