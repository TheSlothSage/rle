systems.the_master = {}

systems.the_master.functions = { "Init", "Update" }

function systems.the_master.Init ()
        rle.this():getComponent("texture"):setStr("path", "assets/special/people/the_master.png") 
end

function systems.the_master.Update () 
     if rle.this():getComponent("keyboard"):getStr("key_down") == "P" then
        rle.debug.Log("printing entities... \n----------------------------------------------------------")
        tilemap = rle.this():getTileMap()
        for x = 0, tilemap.x - 1, 1 do
            for y = 0, tilemap.y - 1, 1 do
                tile = tilemap:getTile(x, y, rle.this().z)
                for index = 0, tile.entities - 1, 1 do
                    entity = tile:getEntity(index)
                    rle.debug.Log(entity.name .. "\t(" .. entity.x .. ", " .. entity.y .. ")")
                end
            end
        end
        rle.debug.Log("done... \n----------------------------------------------------------")
    end
end
