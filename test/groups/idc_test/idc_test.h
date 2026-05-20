#include "tests.h"
#include "tests_includes.h"


/////////////////////////////////////////////////////////////////////
//TESTING_FUNCTION

uint16_t create_idc_frame(
	uint8_t volatile* virtual_addr, 
	uint8_t* real_addr, 
	uint16_t virtual_len, 
	uint16_t real_len, 
	uint16_t virt_pack_num
);
