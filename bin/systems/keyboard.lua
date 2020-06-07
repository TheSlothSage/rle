systems.player = {}
systems.keyboard = {}
systems.keyboard.functions = { "Init", "Update" }

function systems.keyboard.Init ()
end

function systems.keyboard.Update ()
    keyboard_component = rle.this():getComponent("keyboard")
    keyboard_component:setStr("key_down", rle.getKeyDown())
end

