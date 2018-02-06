#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "bluetooth/BluetoothCtl.h"
#include "bluetooth/GattTool.h"
#include "utils/fderror.h"
#include "utils/File.h"
#include "utils/String.h"

#define nbelem(x) (sizeof(x)/sizeof(x[0]))

void tiBluetoothSensorTag_deviceFoundCallback(unsigned64 bluetooth_address, const char* friendly_name);
void tiBluetoothSensorTag_attributeCallback(unsigned64 bluetooth_address, int attribute, int len, const unsigned8* data);

#endif
