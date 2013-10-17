#include <dev/fbgfx.h>

unsigned char low_raw_gz[] = {
  0x1f, 0x8b, 0x08, 0x08, 0x5b, 0x1c, 0xf6, 0x51, 0x00, 0x03, 0x6c, 0x6f,
  0x77, 0x2e, 0x72, 0x61, 0x77, 0x00, 0xed, 0xd4, 0x31, 0x6e, 0x53, 0x01,
  0x14, 0x44, 0x51, 0xe3, 0x04, 0xf1, 0x0b, 0x4b, 0x94, 0x34, 0x50, 0xd1,
  0xb2, 0x01, 0x5b, 0x72, 0x91, 0x9d, 0xb3, 0x1f, 0x36, 0x41, 0x4f, 0x9b,
  0xf9, 0x62, 0xa2, 0x39, 0x47, 0x6f, 0x01, 0xf3, 0x9a, 0xfb, 0xf6, 0x06,
  0xd0, 0xe5, 0x38, 0x8e, 0xcb, 0x09, 0xfe, 0xf7, 0x5b, 0x00, 0xff, 0x92,
  0x3b, 0x60, 0x84, 0xdc, 0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0,
  0x08, 0xb9, 0x03, 0x46, 0xc8, 0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72,
  0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84, 0xdc, 0x01, 0x23, 0xe4, 0x0e, 0x18,
  0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46, 0xc8, 0x1d, 0x30, 0x42, 0xee,
  0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84, 0xdc, 0x01, 0x23,
  0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46, 0xc8, 0x1d,
  0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84,
  0xdc, 0x01, 0x23, 0x3e, 0x4a, 0xee, 0x9e, 0xcf, 0xe7, 0x19, 0x3b, 0x81,
  0x4e, 0xf7, 0xfb, 0x3d, 0x9e, 0x11, 0xb9, 0x03, 0x0a, 0xc9, 0x1d, 0x30,
  0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84, 0xdc,
  0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46,
  0xc8, 0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b,
  0x60, 0xc4, 0x19, 0xb9, 0xbb, 0xdd, 0x6e, 0xaf, 0x69, 0x17, 0xb9, 0x03,
  0xde, 0xe7, 0x8c, 0xdc, 0x9d, 0xe1, 0x22, 0x77, 0xc0, 0xfb, 0xc8, 0x1d,
  0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84,
  0xdc, 0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03,
  0x46, 0xc8, 0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90,
  0x3b, 0x60, 0x84, 0xdc, 0xc5, 0xfd, 0xfa, 0xfc, 0xe5, 0xcf, 0xf7, 0x9f,
  0xd9, 0xfb, 0xfd, 0xed, 0xc7, 0x19, 0x53, 0x61, 0x8a, 0xdc, 0xc5, 0xc9,
  0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77,
  0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41,
  0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d,
  0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92,
  0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee,
  0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b,
  0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e,
  0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9,
  0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e,
  0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8,
  0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93,
  0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72,
  0x17, 0x27, 0x77, 0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d,
  0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71,
  0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9,
  0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77,
  0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41,
  0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d,
  0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92,
  0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee,
  0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b,
  0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e,
  0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9,
  0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e,
  0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8,
  0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93,
  0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72,
  0x17, 0x27, 0x77, 0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d,
  0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71,
  0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9,
  0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77,
  0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41,
  0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d,
  0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92,
  0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee,
  0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b,
  0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e,
  0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9,
  0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e,
  0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8,
  0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93,
  0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72,
  0x17, 0x27, 0x77, 0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d,
  0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71,
  0x72, 0x07, 0x9d, 0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9,
  0x1d, 0x74, 0x92, 0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77,
  0xd0, 0x49, 0xee, 0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41,
  0x27, 0xb9, 0x8b, 0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d,
  0xe4, 0x2e, 0x4e, 0xee, 0xa0, 0x93, 0xdc, 0xc5, 0xc9, 0x1d, 0x74, 0x92,
  0xbb, 0x38, 0xb9, 0x83, 0x4e, 0x72, 0x17, 0x27, 0x77, 0xd0, 0x49, 0xee,
  0xe2, 0xe4, 0x0e, 0x3a, 0xc9, 0x5d, 0x9c, 0xdc, 0x41, 0x27, 0xb9, 0x8b,
  0x93, 0x3b, 0xe8, 0x24, 0x77, 0x71, 0x72, 0x07, 0x9d, 0xe4, 0x2e, 0xee,
  0xe5, 0x72, 0xf9, 0x7a, 0xbd, 0x66, 0xef, 0xf6, 0xe9, 0x7a, 0xc6, 0x54,
  0x98, 0x22, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46, 0xc8, 0x1d, 0x30, 0x42,
  0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84, 0xdc, 0x01,
  0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46, 0xc8,
  0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60,
  0xc4, 0x19, 0xb9, 0x3b, 0x8e, 0xe3, 0x8c, 0xa9, 0xf1, 0x9d, 0x72, 0x07,
  0x53, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46, 0xc8,
  0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60,
  0x84, 0xdc, 0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9,
  0x03, 0x46, 0x8c, 0xe7, 0xee, 0x15, 0x98, 0xf1, 0x78, 0x3c, 0xe2, 0x19,
  0xf9, 0x28, 0xb9, 0x03, 0x78, 0x27, 0xb9, 0x03, 0x46, 0xc8, 0x1d, 0x30,
  0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b, 0x60, 0x84, 0xdc,
  0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46,
  0xc8, 0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07, 0x8c, 0x90, 0x3b,
  0x60, 0x84, 0xdc, 0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21, 0x77, 0xc0, 0x08,
  0xb9, 0x03, 0x46, 0xc8, 0x1d, 0x30, 0x42, 0xee, 0x80, 0x11, 0x72, 0x07,
  0x8c, 0x90, 0x3b, 0x60, 0x84, 0xdc, 0x01, 0x23, 0xe4, 0x0e, 0x18, 0x21,
  0x77, 0xc0, 0x08, 0xb9, 0x03, 0x46, 0xc8, 0x1d, 0x30, 0x42, 0xee, 0x80,
  0x11, 0x27, 0xe5, 0xee, 0x2f, 0x4d, 0x75, 0x4d, 0xd2, 0x20, 0x51, 0x04,
  0x00
};
unsigned int low_raw_gz_len = 1153;

struct fbgfx_image image_low = {
  421, 224, 3, 2, 0, low_raw_gz
};
