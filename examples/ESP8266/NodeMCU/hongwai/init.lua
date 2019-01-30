--init.lua
print("set up wifi mode")

wifi.setmode(wifi.STATION)

station_cfg={}
--here SSID and PassWord should be modified according your wireless router
station_cfg.ssid="Bigiot"
station_cfg.pwd="Bigiot.net"
station_cfg.save=true
wifi.sta.config(station_cfg)

wifi.sta.connect()
tmr.alarm(1, 1000, 1, function()
if wifi.sta.getip()== nil then
print("IP unavaiable, Waiting...")
else
tmr.stop(1)
print("Config done, IP is "..wifi.sta.getip())
dofile("hongwai.lua")
end
end)