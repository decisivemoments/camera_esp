#include "esp_err.h"

typedef enum {
    SPP_DATA_TYPE_HD,    // 高清图像
    SPP_DATA_TYPE_LD     // 低清图像
} spp_data_type_t;

void initialize_spp(void);

esp_err_t send_image_data(uint8_t *data, size_t length, spp_data_type_t type);