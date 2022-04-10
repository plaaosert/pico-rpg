add_executable(rpg
        rpg.c
        external/fonts.c
        external/st7735.c
        external/DEV_Config.c
	external/ICM20948.c
        )

# pull in common dependencies
target_link_libraries(
  rpg
  hardware_pwm
  hardware_pio
  hardware_spi
  hardware_i2c
  pico_stdlib
)

pico_enable_stdio_usb(rpg 1)
pico_enable_stdio_uart(rpg 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(rpg)

# add url via pico_set_program_url
example_auto_set_url(rpg)
