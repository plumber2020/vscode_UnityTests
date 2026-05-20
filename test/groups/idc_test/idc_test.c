#include "idc_test.h"


/////////////////////////////////////////////////////////////////////
//TESTING_DATA

typedef struct {
	uint8_t* volatile _virtual_addr;
	uint8_t* 					_real_addr; 
	uint16_t 					_virtual_len; 
	uint16_t 					_real_len;
	uint16_t 					_virt_pack_num;
} idc_frame_t;


static idc_frame_t INPUT[] = {
	{(uint8_t*)0x1, (uint8_t*)0x1, 1, 1, 1}

};
static uint16_t EXPECTED_res[] = { 
	100 

};

/////////////////////////////////////////////////////////////////////
//TEST_HELPERS

static 
uint16_t execute_create_idc_frame(idc_frame_t* data)
{
	return create_idc_frame(
		data->_virtual_addr,
		data->_real_addr,
		data->_virtual_len,
		data->_real_len,
		data->_virt_pack_num
	);
}

static 
void printINPUT_create_idc_frame(idc_frame_t* in)
{
	printf("virtual_addr = %p, ",	in->_virtual_addr);
	printf("real_addr = %p, ", 		in->_real_addr);
	printf("virtual_len = %u, ", 	in->_virtual_len);
	printf("real_len = %u,", 			in->_real_len);
	printf("virt_pack_num = %u:", in->_virt_pack_num);
}
static 
void printEXPECTED_create_idc_frame(uint16_t res)
{
	printf("res = 0:res = %u:", res);
}

/////////////////////////////////////////////////////////////////////
//TEST_FUNCTION

void create_idc_frame_test_1()
{
	int i = 0;
	uint16_t res = execute_create_idc_frame(&INPUT[i]);

#if !defined(coverageTest)
    printf("vikm468332010:Õ¿œ»—¿“‹:create_idc_frame:");
    printINPUT_create_idc_frame(&INPUT[i]);
    TEST_ASSERT_EQUAL_INT(EXPECTED_res[i], res);
    printEXPECTED_create_idc_frame(res);
#endif
}