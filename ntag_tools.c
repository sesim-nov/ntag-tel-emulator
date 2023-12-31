#include "ntag_tools.h"
#include <lib/nfc/helpers/nfc_generators.h>

const NfcGenerator* const* get_ntag_generator() {
    const NfcGenerator* const* gen = nfc_generators;
    for (; gen != NULL; ++gen) {
        if (!strcmp((*gen)->name,"NTAG213")) {
            break;
        }
    }
    FURI_LOG_D("NtagTelEmulator", "Name: %s", (*gen)->name);
    return gen;
}

void ntag_213_init_blank_tag(NfcDevice* nfc) {
    nfc->format = NfcDeviceSaveFormatMifareUl;

    const NfcGenerator* const* gen = get_ntag_generator();
    furi_assert(gen);
    (*gen)->generator_func(&nfc->dev_data);
}

void print_str(char *s) {
	int i = 0;
	while (s[i] != 0x0) {
		if (i > 0 && i % 4 == 0) {
			printf("\n");
		}
		printf("%02x ", s[i] & 0xff);
		i++;
	}
}

void construct_ndef_record_header(uint8_t* buf, size_t buf_len, size_t payload_len) {
    // Length check.
    if (buf_len < 4) {
        return;
    }
	// Byte 0: TNF byte
	buf[0] = 0x1 << 7; // This is the first message
	buf[0] = (0x1 << 6) | buf[0]; // This is also the last message, it's just a url.
	// not setting flag 5 for chunk flag
	buf[0] = (0x1 << 4) | buf[0]; // This is a short record (1-byte payload length). 
	// Not setting flag 3 for ID length. No ID will be provided. 
	buf[0] = 0x1 | buf[0]; // Setting bits 2-0 for the TNF, which is "Well Known" for URL's. 
	
	// Byte 1: Type Length
	buf[1] = 0x1; // Type name is 1 byte long
	
	// Byte 2: Payload Length (Short Record)
	buf[2] = payload_len * sizeof(uint8_t); 
	
	// Byte 3: Payload Type
	buf[3] = (uint8_t)'U'; 
}

size_t create_ndef_tel_record(uint8_t* buf, size_t buf_len, uint8_t* payload, size_t payload_len) {
	uint8_t* mod_payload = malloc((payload_len + 2) * sizeof(uint8_t));
	mod_payload[0] = 0x05; // Append tel:// NFC shorthand to front of payload
    size_t mod_payload_len = 1;
	
	memcpy(mod_payload + 1, payload, payload_len * sizeof(uint8_t));
    mod_payload_len += payload_len;

	// Construct data header
	size_t header_len = 4;
	uint8_t* header = malloc(header_len * sizeof(uint8_t)); 
	construct_ndef_record_header(header, header_len, mod_payload_len);

	// Build message
	size_t total_len = header_len + mod_payload_len;
	if (buf_len < total_len) {
		FURI_LOG_D("NtagEmulator", "Message exceeds provided buffer. Increase buffer size.");
		return 0;
	}
    size_t cursor = 0;
	memcpy(buf, header, header_len * sizeof(uint8_t));
    cursor += header_len;

	memcpy(buf + cursor, mod_payload, mod_payload_len * sizeof(uint8_t));
    cursor += mod_payload_len;

	free(mod_payload);
	free(header);

    return cursor;
}

size_t create_tlv_record_for_message(uint8_t* buf, size_t buf_len, uint8_t* message, size_t msg_length) {
    if (msg_length > ((2 << 7) - 3)) {
		FURI_LOG_D("NtagEmulator", "Message too big");
        return 0;
    } else if (buf_len < (uint8_t)(msg_length + 3)) {
		FURI_LOG_D("NtagEmulator", "Buffer too small");
		return 0;
	}
    
	uint8_t t = 0x03;
	uint8_t l = (uint8_t)msg_length;
	uint8_t terminator = 0xfe;

    uint8_t cursor = 1;
	buf[0] = t; // Start with T
	
	memcpy(buf + cursor, &l, 1); // Append L
	cursor++;

	memcpy(buf + cursor, message, msg_length * sizeof(uint8_t)); // Append V (message)
	cursor += msg_length;

	memcpy(buf + cursor, &terminator, 1); // Append terminator byte
    cursor ++;

    return cursor;
}

size_t ndef_encode_phone_number(uint8_t* buf, size_t buf_len, uint8_t* number, uint8_t number_len) {
    size_t tel_record_len = number_len + 10;
    uint8_t* tel_record = malloc(tel_record_len * sizeof(uint8_t));
    tel_record_len = create_ndef_tel_record(tel_record, tel_record_len, number, number_len);

    size_t out = create_tlv_record_for_message(buf, buf_len, tel_record, tel_record_len);
    free(tel_record);
    return out;
}
//    uint8_t new_data[] = {
//                           0x04, 0x30, 0xD2, 0x6E,
//                           0xEB, 0xA1, 0x47, 0x88,
//                           0x85, 0x48, 0x00, 0x00,
//                           0xE1, 0x10, 0x12, 0x00,
//                           0x03, 0x0f, 0xd1, 0x01,
//                           0x0b, 0x55, 0x05, 0x35,
//                           0x35, 0x35, 0x38, 0x36,
//                           0x37, 0x35, 0x33, 0x30,
//                           0x39, 0xfe, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0x00,
//                           0x00, 0x00, 0x00, 0xBD,
//                           0x04, 0x00, 0x00, 0xFF,
//                           0x00, 0x05, 0x00, 0x00,
//                           0xFF, 0xFF, 0xFF, 0xFF,
//                           0x00, 0x00, 0x00, 0x00
//                           };
