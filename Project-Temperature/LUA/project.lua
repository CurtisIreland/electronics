-- setup SPI and connect display
function init_spi_display()
    -- Hardware SPI CLK  = GPIO14
    -- Hardware SPI MOSI = GPIO13
    -- Hardware SPI MISO = GPIO12 (not used)
    -- Hardware SPI /CS  = GPIO15 (not used)
    -- CS, D/C, and RES can be assigned freely to available GPIOs
    local cs  = 8 -- GPIO15, pull-down 10k to GND
    local dc  = 4 -- GPIO2
    local res = 3 -- GPIO16

    spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_LOW, 8, 8)
    -- we won't be using the HSPI /CS line, so disable it again
    gpio.mode(8, gpio.INPUT, gpio.PULLUP)

    -- initialize the matching driver for your display
    disp = ucg.ili9341_18x240x320_hw_spi(cs, dc, res)
end

function draw_screen()
    w = disp:getWidth();
    h = disp:getHeight();

    disp:setColor(255, 255, 255);
    disp:drawRFrame(2, 2, (w - 2), (h - 2), 0);

    disp:setFont(ucg.font_ncenR14_hr)
    disp:setColor(0, 165, 80);
    disp:drawString(5, (5 + 14), 0, "Temperature")
    disp:drawString(5, (h / 2 + (14 / 2)), 0, "Humidity")

    disp:setColor(192, 0, 0);
    disp:drawHLine(5, (5 + 14 + 4), (w - 10))
    disp:drawHLine(5, (5 + 14 + 5), (w - 10))
    disp:drawHLine(5, ((h / 2 + (14 / 2)) + 4), (w - 10))
    disp:drawHLine(5, ((h / 2 + (14 / 2)) + 5), (w - 10))
end

function show_value()
    w = disp:getWidth()
    h = disp:getHeight()

    status, temp, humi, temp_dec, humi_dec = dht.read(dht_pin)
    tem = string.format("%d.%01d",
          math.floor(temp),
          temp_dec
    )
    hum = string.format("%d.%01d",
          math.floor(humi),
          humi_dec
    )

    disp:setColor(0, 0, 0)
    disp:drawRBox((w / 2), 30, (w / 2 - 5), 18, 0)
    disp:drawRBox((w / 2), ((h / 2 + (14 / 2)) + 7), (w / 2 - 5), 18, 0)

    disp:setFont(ucg.font_helvB18_hr)
    disp:setColor(26, 116, 188)
    disp:drawString((w / 2), 48, 0, tem.." C")
    disp:drawString((w / 2), ((h / 2 + (14 / 2)) + 25), 0, hum.."%")

end


init_spi_display()

disp:begin(ucg.FONT_MODE_TRANSPARENT)
disp:setFont(ucg.font_ncenR14_hr)
disp:clearScreen()
disp:setRotate270()

-- Draw initial screen
draw_screen();

dht_pin = 2
--show_value("20.0", "37.2");

-- change 5000 to something more sane
if not tmr.create():alarm(5000, tmr.ALARM_AUTO, show_value)
then
  print("Problem starting timer")
end