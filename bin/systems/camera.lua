systems.camera = {}

systems.camera.functions = { "Init", "Update" }

function systems.camera.Init ()
        -- set the camera rect size to the max_tileX/Y 
        rle.this():getComponent("camera"):setNum("max_x", rle.sdl_getMax_X())
        rle.this():getComponent("camera"):setNum("max_y", rle.sdl_getMax_Y())
end

function systems.camera.Update ()

        -- might as well update this
        rle.this():getComponent("camera"):setNum("max_x", rle.sdl_getMax_X())
        rle.this():getComponent("camera"):setNum("max_y", rle.sdl_getMax_Y())
	-- get the tilemap and start grabbing entities
        tilemap = rle.this():getTileMap() 
        
        -- init
        max_x = rle.this():getComponent("camera"):getNum("max_x")
        max_y = rle.this():getComponent("camera"):getNum("max_y")

        tile_x = rle.this():getComponent("camera"):getNum("tile_x")
        tile_y = rle.this():getComponent("camera"):getNum("tile_y")

        -- max tile must be an integer, so the limit uses the rounding formula
        -- (a - a%b)/b = round(a/b)

        -- Center of screen
        offset_x = (max_x - max_x % 2)/2
        offset_y = (max_y - max_y % 2)/2

        --get farthest X from camera
        tilemap_offset_x = offset_x

        --check if this object has a texture 
        if(rle.this():checkComponent("texture")) then
                rle.sdl_pushTexture(offset_x, offset_y, rle.this():getComponent("texture"):getStr("path"))
        end

        
 
        rle.sdl_update()
end
