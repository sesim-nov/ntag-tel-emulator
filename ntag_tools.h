#pragma once

#include <lib/nfc/nfc_device.h>

void ntag_213_init_blank_tag(NfcDevice* nfc);

size_t ndef_encode_phone_number(uint8_t* buf, size_t buf_len, uint8_t* number, uint8_t number_len);
