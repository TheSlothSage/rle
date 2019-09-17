systems.camera = {}

systems.camera.functions = { "Init", "Update" }

function systems.camera.Init ()
        -- set the camera rect size to the max_tileX/Y 
        rle.this():getComponent("camera"):setNum("max_x", rle.sdl_getMax_X())
        rle.this():getComponent("camera"):setNum("max_y", rle.sdl_getMax_Y())
end

function systems.camera.Update ()
        
end
