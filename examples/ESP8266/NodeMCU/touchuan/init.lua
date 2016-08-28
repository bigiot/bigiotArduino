--init.lua web config
print("set up wifi mode")
enduser_setup.start(
  function()
    print("Connected to wifi as:" .. wifi.sta.getip())
    dofile("main.lua")
  end,
  function(err, str)
    print("enduser_setup: Err #" .. err .. ": " .. str)
  end
)