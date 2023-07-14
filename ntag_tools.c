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

void construct_ndef_record_header(char* buf, size_t buf_len, size_t payload_len) {
    // Length check.
    if (buf_len < payload_len) {
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
	buf[2] = payload_len; // Let's reserve 16 bytes for now. 
	
	// Byte 3: Payload Type
	buf[3] = 'U'; 
}

void create_ndef_tel_record(char* buf, size_t buf_len, char* payload) {
	char* mod_payload = calloc(strlen(payload) + 2, sizeof(char));
	mod_payload[0] = 0x05; // Append tel:// NFC shorthand to front of payload
	
	mod_payload = strcat(mod_payload, payload);

	// Construct data header
	size_t header_size = 5;
	size_t payload_size = strlen(mod_payload);
	char* header = calloc(header_size, sizeof(char)); 
	construct_ndef_record_header(header, header_size, payload_size);

	// Build message
	size_t total_size = header_size + payload_size + 1;
	if (buf_len < total_size) {
		printf("Message exceeds provided buffer. Increase buffer size.");
		return;
	}
	strcpy(buf, header);
	strcat(buf, mod_payload);

	free(mod_payload);
	free(header);
}

size_t create_tlv_record_for_message(uint8_t* buf, size_t buf_len, uint8_t* message, size_t msg_length) {
    if (msg_length > ((2 << 7) - 3)) {
        return 0;
    } else if (buf_len < (uint8_t)(msg_length + 3)) {
		printf("Buffer too small");
		return 0;
	}
    
	uint8_t t = 0x03;
	uint8_t l = (uint8_t)msg_length;
	uint8_t terminator = 0xfe;

    uint8_t cursor = 1;
	buf[0] = t; // Start with T
	
	memcpy(buf + cursor, &l, 1); // Append L
	cursor++;

	memcpy(buf + cursor, message, msg_length); // Append V (message)
	cursor += msg_length;

	memcpy(buf + cursor, &terminator, 1); // Append terminator byte
    cursor ++;

    return cursor;
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
