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

        -- max tile must be an integer, so the limit uses the rounding formula
        -- (a - a%b)/b = round(a/b)
        for i = rle.this().x - (max_x - max_x % 2)/2, rle.this().x + (max_x - max_x % 2)/2, 1  
        do
                for j = rle.this().y - (max_y - max_y % 2)/2, rle.this().y + (max_x - max_x % 2)/2, 1
                do
                        if i >= 0 and j >= 0 then
                                
                                tile = tilemap:getTile(i, j, rle.this().z)
                                
                                
                                for index = 0, tile.entities - 1, 1
                                do
                                        entity = tile:getEntity(index) 

                                        if entity:checkComponent("texture") then
                                                rle.sdl_pushTexture(i, j, entity:getComponent("texture"):getStr("path"))
                                        end
                                        
                                end
                                
                        end
                end
        end
        rle.sdl_update()
end
