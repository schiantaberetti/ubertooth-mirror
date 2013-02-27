BT_UTILS_DIR = ../bluetooth_rxtx/
COMMON_DIR = ../common

all: bt_utils
	cc -I . -I $(BT_UTILS_DIR) -o test_bt_utils test_bt_utils.c bt_utils.o 
	
bt_utils:
	cc -I . -c $(BT_UTILS_DIR)/bt_utils.c $(BT_UTILS_DIR)/bt_utils.h 
