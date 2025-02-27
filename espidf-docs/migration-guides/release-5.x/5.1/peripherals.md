# Peripherals




## GPSPI


Following items are deprecated. Since ESP-IDF v5.1, GPSPI clock source is configurable.


* `spi_get_actual_clock` is deprecated, you should use `spi_device_get_actual_freq()` instead.




## LEDC


* `soc_periph_ledc_clk_src_legacy_t::LEDC_USE_RTC8M_CLK` is deprecated. Please use `LEDC_USE_RC_FAST_CLK` instead.